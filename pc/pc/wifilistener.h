#ifndef WIFILISTENER_H
#define WIFILISTENER_H

#include <QByteArray>
#include <QHash>
#include <QMetaType>
#include <QObject>
#include <QString>

class QTcpServer;
class QTcpSocket;

/*
 * WifiListener 实现上位机侧的 TCP 服务器。
 *
 * ESP32 端作为 TCP 客户端主动连接本机监听端口，每条消息一行：
 *     id=1,len=5,text=hello\n
 *
 * 监听和收发都在 GUI 线程中异步完成（QTcpServer 本身是事件驱动的），
 * 不需要额外工作线程。多个客户端可以同时连接，每个客户端有独立缓冲。
 */
class WifiListener : public QObject
{
    Q_OBJECT

public:
    struct Message
    {
        bool parsed = false;     // 是否成功解析出 id/len/text
        quint32 id = 0;
        quint32 len = 0;
        QString text;
        QString rawLine;         // 一行原始内容（不含行尾 \r\n）
        QString peer;            // "ip:port"
    };

    explicit WifiListener(QObject *parent = nullptr);
    ~WifiListener() override;

    bool isListening() const;
    int clientCount() const;

    // 启动/停止监听；失败时通过 error 返回原因，同时发出 listeningFailed
    bool start(quint16 port, QString *error = nullptr);
    void stop();

signals:
    void listeningStarted(quint16 port);
    void listeningFailed(const QString &error);
    void listeningStopped();
    void clientConnected(const QString &peer);
    void clientDisconnected(const QString &peer);
    void messageReceived(const WifiListener::Message &message);
    void logMessage(const QString &message, int level);   // 仅用于内部异常提示

private slots:
    void handleNewConnection();
    void handleReadyRead();
    void handleDisconnected();

private:
    static Message parseLine(const QString &line, const QString &peer);
    static QString peerString(const QTcpSocket *socket);
    void closeAllClients();

    QTcpServer *m_server = nullptr;
    QHash<QTcpSocket *, QByteArray> m_buffers;
};

Q_DECLARE_METATYPE(WifiListener::Message)

#endif // WIFILISTENER_H
