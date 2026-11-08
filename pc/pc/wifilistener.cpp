#include "wifilistener.h"
#include "otaprotocol.h"

#include <QAbstractSocket>
#include <QHostAddress>
#include <QRegularExpression>
#include <QTcpServer>
#include <QTcpSocket>

namespace {

const int kMaxLineBuffer = 64 * 1024;   // 单行缓冲上限，防止异常数据撑爆内存

} // namespace

WifiListener::WifiListener(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<WifiListener::Message>("WifiListener::Message");
}

WifiListener::~WifiListener()
{
    // 析构时静默清理，不发射信号，避免访问已经销毁的界面对象
    if (m_server)
        m_server->close();
    closeAllClients();
}

bool WifiListener::isListening() const
{
    return m_server && m_server->isListening();
}

int WifiListener::clientCount() const
{
    return m_buffers.size();
}

bool WifiListener::start(quint16 port, QString *error)
{
    if (isListening())
        return true;

    if (!m_server) {
        m_server = new QTcpServer(this);
        connect(m_server, &QTcpServer::newConnection,
                this, &WifiListener::handleNewConnection);
    }

    // ESP32 通过局域网地址连接，绑定所有 IPv4 网卡即可
    if (!m_server->listen(QHostAddress::AnyIPv4, port)) {
        const QString message = m_server->errorString();
        if (error)
            *error = message;
        emit listeningFailed(message);
        return false;
    }

    emit listeningStarted(m_server->serverPort());
    return true;
}

void WifiListener::stop()
{
    const bool wasListening = isListening();

    if (m_server)
        m_server->close();
    closeAllClients();

    if (wasListening)
        emit listeningStopped();
}

void WifiListener::closeAllClients()
{
    // 先清掉还没被 accept 的连接
    if (m_server) {
        while (m_server->hasPendingConnections()) {
            QTcpSocket *socket = m_server->nextPendingConnection();
            if (!socket)
                break;
            socket->abort();
            socket->deleteLater();
        }
    }

    const QList<QTcpSocket *> clients = m_buffers.keys();
    for (QTcpSocket *socket : clients) {
        m_buffers.remove(socket);
        socket->disconnect(this);   // 不再处理它的 readyRead / disconnected
        socket->abort();
        socket->deleteLater();
    }
    m_buffers.clear();
}

void WifiListener::handleNewConnection()
{
    if (!m_server)
        return;

    while (m_server->hasPendingConnections()) {
        QTcpSocket *socket = m_server->nextPendingConnection();
        if (!socket)
            break;

        m_buffers.insert(socket, QByteArray());
        connect(socket, &QTcpSocket::readyRead, this, &WifiListener::handleReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &WifiListener::handleDisconnected);

        // 关闭 Nagle，降低小报文延迟
        socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

        emit clientConnected(peerString(socket));
    }
}

void WifiListener::handleReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket || !m_buffers.contains(socket))
        return;

    QByteArray &buffer = m_buffers[socket];
    buffer.append(socket->readAll());

    int start = 0;
    int newline = -1;
    while ((newline = buffer.indexOf('\n', start)) >= 0) {
        QByteArray raw = buffer.mid(start, newline - start);
        start = newline + 1;

        if (raw.endsWith('\r'))
            raw.chop(1);
        if (raw.isEmpty())
            continue;

        const QString line = QString::fromUtf8(raw);
        emit messageReceived(parseLine(line, peerString(socket)));
    }

    if (start > 0)
        buffer.remove(0, start);

    if (buffer.size() > kMaxLineBuffer) {
        emit logMessage(QStringLiteral("客户端 %1 的单行数据超过 %2 字节，已丢弃该缓冲区。")
                            .arg(peerString(socket))
                            .arg(kMaxLineBuffer),
                        Ota::LogWarning);
        buffer.clear();
    }
}

void WifiListener::handleDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket)
        return;

    const QString peer = peerString(socket);
    m_buffers.remove(socket);
    socket->deleteLater();

    emit clientDisconnected(peer);
}

WifiListener::Message WifiListener::parseLine(const QString &line, const QString &peer)
{
    Message message;
    message.rawLine = line;
    message.peer = peer;

    // 与 ESP32 wifi_task.c 的输出格式保持一致：
    //     id=%u,len=%u,text=%s\n
    static const QRegularExpression re(
        QStringLiteral("^id=(\\d+),len=(\\d+),text=(.*)$"));

    const QRegularExpressionMatch match = re.match(line);
    if (!match.hasMatch())
        return message;

    bool ok = false;
    const quint32 id = match.captured(1).toUInt(&ok);
    if (!ok)
        return message;

    const quint32 len = match.captured(2).toUInt(&ok);
    if (!ok)
        return message;

    message.parsed = true;
    message.id = id;
    message.len = len;
    message.text = match.captured(3);
    return message;
}

QString WifiListener::peerString(const QTcpSocket *socket)
{
    if (!socket)
        return QStringLiteral("unknown");
    return QStringLiteral("%1:%2")
        .arg(socket->peerAddress().toString())
        .arg(socket->peerPort());
}
