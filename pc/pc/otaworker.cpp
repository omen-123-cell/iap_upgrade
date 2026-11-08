#include "otaworker.h"
#include "otaprotocol.h"

#include <QElapsedTimer>
#include <QSerialPort>
#include <QThread>

OtaWorker::OtaWorker(QObject *parent)
    : QObject(parent)
    , m_cancel(false)
{
}

OtaWorker::~OtaWorker()
{
    closePortInternal();
}

void OtaWorker::requestCancel()
{
    m_cancel.store(true);
}

void OtaWorker::openPort(const QString &portName, int baudRate, int dataBits,
                         int stopBits, int parity)
{
    closePortInternal();

    m_port = new QSerialPort(this);
    m_port->setPortName(portName);
    m_port->setBaudRate(baudRate);
    m_port->setDataBits(static_cast<QSerialPort::DataBits>(dataBits));
    m_port->setStopBits(static_cast<QSerialPort::StopBits>(stopBits));
    m_port->setParity(static_cast<QSerialPort::Parity>(parity));
    m_port->setFlowControl(QSerialPort::NoFlowControl);

    if (!m_port->open(QIODevice::ReadWrite)) {
        const QString error = m_port->errorString();
        delete m_port;
        m_port = nullptr;
        emit portOpenFailed(error);
        return;
    }

    m_port->clear(QSerialPort::AllDirections);

    const QString description = QStringLiteral("%1 @ %2 8N1").arg(portName).arg(baudRate);
    emit portOpened(description);
}

void OtaWorker::closePort()
{
    closePortInternal();
    emit portClosed();
}

void OtaWorker::closePortInternal()
{
    if (!m_port)
        return;

    if (m_port->isOpen())
        m_port->close();
    delete m_port;
    m_port = nullptr;
}

bool OtaWorker::ensurePortOpen(QString *error) const
{
    if (m_port && m_port->isOpen())
        return true;
    if (error)
        *error = QStringLiteral("串口未打开，请先打开串口。");
    return false;
}

void OtaWorker::readVersion(int timeoutMs)
{
    m_cancel.store(false);

    QString error;
    if (!ensurePortOpen(&error)) {
        emit versionResult(false, QByteArray(), error);
        return;
    }

    if (!handshake(timeoutMs, &error)) {
        emit versionResult(false, QByteArray(), error);
        return;
    }

    emit partitionChanged(QStringLiteral("Bootloader"));

    QByteArray payload;
    if (!queryVersion(timeoutMs, &payload, &error)) {
        emit versionResult(false, QByteArray(), error);
        return;
    }

    emit versionResult(true, payload, Ota::versionString(payload));
}

bool OtaWorker::handshake(int timeoutMs, QString *error)
{
    quint8 status = 0xFF;
    if (!request(Ota::CmdWaitUpgrade, Ota::buildFrame(Ota::CmdWaitUpgrade),
                 timeoutMs, QStringLiteral("WAIT_UPGRADE"), &status, nullptr, error)) {
        return false;
    }

    if (status != 0x00) {
        if (error)
            *error = QStringLiteral("握手失败：%1").arg(Ota::statusText(status));
        return false;
    }
    return true;
}

bool OtaWorker::queryVersion(int timeoutMs, QByteArray *payload, QString *error)
{
    quint8 status = 0xFF;
    if (!request(Ota::CmdGetVersion, Ota::buildFrame(Ota::CmdGetVersion),
                 timeoutMs, QStringLiteral("GET_FIRM_VERSION"), &status, payload, error)) {
        return false;
    }

    if (status != 0x00) {
        if (error)
            *error = QStringLiteral("版本查询失败：%1").arg(Ota::statusText(status));
        return false;
    }
    return true;
}

void OtaWorker::startUpgrade(const QByteArray &image, int chunkSize,
                             int responseTimeoutMs, int eraseTimeoutMs,
                             int endTimeoutMs, int frameDelayMs)
{
    m_cancel.store(false);

    if (image.isEmpty() || image.size() > 0xFFFF) {
        emit upgradeFinished(false, false,
                             QStringLiteral("固件大小无效：%1 字节（上限 65535 字节）。")
                                 .arg(image.size()));
        return;
    }
    if (chunkSize < 4 || chunkSize > 250 || (chunkSize % 4) != 0) {
        emit upgradeFinished(false, false,
                             QStringLiteral("每帧数据长度必须是 4~250 之间且为 4 的倍数。"));
        return;
    }

    QString error;
    if (!ensurePortOpen(&error)) {
        emit upgradeFinished(false, false, error);
        return;
    }

    const quint16 targetCrc = Ota::crc16Modbus(image);
    const int total = image.size();

    emit logMessage(QStringLiteral("发送大小 %1 字节，目标 CRC %2，每帧 %3 字节。")
                        .arg(total)
                        .arg(Ota::crcHex(targetCrc))
                        .arg(chunkSize),
                    Ota::LogInfo);

    // ---------- 1. 握手 ----------
    emit logMessage(QStringLiteral("握手 (WAIT_UPGRADE) ..."), Ota::LogInfo);
    if (!handshake(responseTimeoutMs, &error)) {
        if (m_cancel.load()) {
            finishCancelled();
        } else {
            emit upgradeFinished(false, false, error);
        }
        return;
    }
    emit logMessage(QStringLiteral("握手成功。"), Ota::LogSuccess);
    emit partitionChanged(QStringLiteral("Bootloader"));

    // ---------- 版本查询（失败不影响升级） ----------
    QByteArray version;
    QString versionError;
    if (queryVersion(responseTimeoutMs, &version, &versionError)) {
        emit logMessage(QStringLiteral("Bootloader 版本：%1（原始 %2）")
                            .arg(Ota::versionString(version), Ota::hexString(version)),
                        Ota::LogInfo);
    } else {
        emit logMessage(QStringLiteral("版本查询失败（不影响升级）：%1").arg(versionError),
                        Ota::LogWarning);
    }

    if (m_cancel.load()) {
        finishCancelled();
        return;
    }

    // ---------- 2. BEGIN：擦除 App 区 ----------
    emit logMessage(QStringLiteral("开始传输 (BEGIN_TRANSMIT_FIRM)，擦除 App 区 ..."),
                    Ota::LogInfo);

    quint8 status = 0xFF;
    if (!request(Ota::CmdBeginTransmit,
                 Ota::buildBeginFrame(static_cast<quint16>(total), targetCrc),
                 eraseTimeoutMs, QStringLiteral("BEGIN_TRANSMIT_FIRM"),
                 &status, nullptr, &error)) {
        if (m_cancel.load()) {
            finishCancelled();
            return;
        }
        emit upgradeFinished(false, false,
                             QStringLiteral("擦除阶段失败：%1\n"
                                            "固件只在擦除成功时才回应，超时也可能表示 Flash 擦除失败。"
                                            "请复位设备后重试。")
                                 .arg(error));
        return;
    }
    if (status != 0x00) {
        emit upgradeFinished(false, false,
                             QStringLiteral("擦除失败：%1。请复位设备后重试。")
                                 .arg(Ota::statusText(status)));
        return;
    }
    emit logMessage(QStringLiteral("Flash 已擦除。"), Ota::LogSuccess);

    // ---------- 3. 分包发送 ----------
    emit logMessage(QStringLiteral("发送固件数据 ..."), Ota::LogInfo);

    int offset = 0;
    int packet = 0;
    QElapsedTimer txTimer;
    txTimer.start();

    while (offset < total) {
        if (m_cancel.load()) {
            finishCancelled();
            return;
        }

        const int partSize = qMin(chunkSize, total - offset);
        const QByteArray part = image.mid(offset, partSize);
        const QString label = QStringLiteral("第 %1 帧 @0x%2")
                                  .arg(packet + 1)
                                  .arg(offset, 4, 16, QLatin1Char('0'));
        const QByteArray frame = Ota::buildFrame(Ota::CmdReadFirm, part);

        if (!request(Ota::CmdReadFirm, frame, responseTimeoutMs, label,
                     &status, nullptr, &error)) {
            if (m_cancel.load()) {
                finishCancelled();
            } else {
                emit upgradeFinished(false, false,
                                     QStringLiteral("%1 失败：%2\n"
                                                    "协议没有偏移字段，不能安全重发。"
                                                    "请复位 Bootloader 后重新升级。")
                                         .arg(label, error));
            }
            return;
        }

        if (status != 0x00) {
            emit upgradeFinished(false, false,
                                 QStringLiteral("%1 写入失败：%2\n"
                                                "请复位 Bootloader 后重新升级。")
                                     .arg(label, Ota::statusText(status)));
            return;
        }

        offset += partSize;
        ++packet;

        if (frameDelayMs > 0) {
            sleepInterruptible(frameDelayMs);
            if (m_cancel.load()) {
                finishCancelled();
                return;
            }
        }

        const double elapsed = txTimer.elapsed() / 1000.0;
        const double kbps = elapsed > 0.0 ? (offset / 1024.0) / elapsed : 0.0;
        emit progressChanged(offset, total, kbps);
    }

    // ---------- 4. END：校验并跳转 ----------
    emit logMessage(QStringLiteral("数据发送完成，等待 CRC 校验 ..."), Ota::LogInfo);

    if (!request(Ota::CmdEndTransmit, Ota::buildFrame(Ota::CmdEndTransmit),
                 endTimeoutMs, QStringLiteral("END_TRANSMIT_FIRM"),
                 &status, nullptr, &error)) {
        emit upgradeFinished(false, false,
                             QStringLiteral("结束校验无应答。固件算出的 CRC 与目标 CRC %1 "
                                            "不一致，或设备已断开。请复位后重新升级。")
                                 .arg(Ota::crcHex(targetCrc)));
        return;
    }

    if (status != 0x00) {
        emit upgradeFinished(false, false,
                             QStringLiteral("结束传输失败：%1。请复位后重新升级。")
                                 .arg(Ota::statusText(status)));
        return;
    }

    emit progressChanged(total, total, 0.0);
    emit partitionChanged(QStringLiteral("App（已跳转）"));
    emit logMessage(QStringLiteral("CRC 校验通过，设备正在跳转到 App。"), Ota::LogSuccess);

    const double seconds = txTimer.elapsed() / 1000.0;
    emit upgradeFinished(true, false,
                         QStringLiteral("升级完成：%1 字节，CRC %2，用时 %3 秒。")
                             .arg(total)
                             .arg(Ota::crcHex(targetCrc))
                             .arg(seconds, 0, 'f', 1));
}

void OtaWorker::finishCancelled()
{
    emit upgradeFinished(false, true,
                         QStringLiteral("升级已取消。设备仍停留在 Bootloader，"
                                        "请复位后重新升级。"));
}

void OtaWorker::sleepInterruptible(int ms)
{
    const int slice = 10;
    for (int waited = 0; waited < ms; waited += slice) {
        if (m_cancel.load())
            return;
        QThread::msleep(static_cast<unsigned long>(qMin(slice, ms - waited)));
    }
}

bool OtaWorker::request(quint8 cmd, const QByteArray &frame, int timeoutMs,
                        const QString &label, quint8 *status, QByteArray *data,
                        QString *error)
{
    if (!ensurePortOpen(error))
        return false;

    if (m_cancel.load()) {
        if (error)
            *error = QStringLiteral("%1：操作已取消").arg(label);
        return false;
    }

    m_port->clear(QSerialPort::Input);

    const qint64 written = m_port->write(frame);
    if (written != frame.size()) {
        if (error) {
            *error = QStringLiteral("%1：发送失败（%2）")
                         .arg(label, m_port->errorString());
        }
        return false;
    }

    const int baud = qMax(1, m_port->baudRate());
    const int writeTimeout = qMax(1000, static_cast<int>(frame.size() * 10.0 * 1000.0 / baud) + 500);
    m_port->waitForBytesWritten(writeTimeout);

    quint8 responseCmd = 0;
    quint8 responseStatus = 0;
    QByteArray responseData;
    QString readError;

    if (!readResponse(timeoutMs, &responseCmd, &responseStatus, &responseData, &readError)) {
        if (error)
            *error = QStringLiteral("%1：%2").arg(label, readError);
        return false;
    }

    const quint8 expectedCmd = static_cast<quint8>(cmd | Ota::AckFlag);
    if (responseCmd != expectedCmd) {
        if (error) {
            *error = QStringLiteral("%1：应答命令 0x%2，预期 0x%3")
                         .arg(label)
                         .arg(responseCmd, 2, 16, QLatin1Char('0'))
                         .arg(expectedCmd, 2, 16, QLatin1Char('0'));
        }
        return false;
    }

    if (status)
        *status = responseStatus;
    if (data)
        *data = responseData;
    return true;
}

bool OtaWorker::readResponse(int timeoutMs, quint8 *cmd, quint8 *status,
                             QByteArray *data, QString *error)
{
    static const QByteArray header = QByteArray::fromHex("AA01");

    QByteArray buffer;
    QElapsedTimer timer;
    timer.start();

    while (timer.elapsed() < timeoutMs) {
        if (m_cancel.load()) {
            if (error)
                *error = QStringLiteral("操作已取消");
            return false;
        }

        const int remain = timeoutMs - static_cast<int>(timer.elapsed());
        if (remain <= 0)
            break;

        if (m_port->waitForReadyRead(qMin(50, remain)))
            buffer.append(m_port->readAll());

        for (;;) {
            const int index = buffer.indexOf(header);
            if (index < 0) {
                // 没有帧头：只保留最后一个字节，它可能是下一个 0xAA
                if (buffer.size() > 1)
                    buffer = buffer.right(1);
                break;
            }
            if (index > 0)
                buffer.remove(0, index);

            if (buffer.size() < 4)
                break;

            const int len = static_cast<quint8>(buffer.at(3));
            const int total = 6 + len;      // 4 字节头 + len + 2 字节 CRC
            if (buffer.size() < total)
                break;

            const QByteArray frame = buffer.left(total);
            const quint16 calcCrc = Ota::crc16Modbus(frame.left(frame.size() - 2));
            const quint16 recvCrc =
                static_cast<quint8>(frame.at(frame.size() - 2))
                | (static_cast<quint16>(static_cast<quint8>(frame.at(frame.size() - 1))) << 8);

            if (calcCrc != recvCrc) {
                // CRC 不正确，丢掉一个字节后重新查找帧头
                buffer.remove(0, 1);
                continue;
            }

            if (cmd)
                *cmd = static_cast<quint8>(frame.at(2));
            if (status)
                *status = static_cast<quint8>(frame.at(4));
            if (data)
                *data = frame.mid(5, frame.size() - 7);
            return true;
        }
    }

    if (error) {
        if (m_cancel.load())
            *error = QStringLiteral("操作已取消");
        else
            *error = QStringLiteral("等待应答超时（%1 ms）").arg(timeoutMs);
    }
    return false;
}
