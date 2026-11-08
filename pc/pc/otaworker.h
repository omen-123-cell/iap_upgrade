#ifndef OTAWORKER_H
#define OTAWORKER_H

#include <QByteArray>
#include <QObject>

#include <atomic>

class QSerialPort;

/*
 * OtaWorker 运行在独立线程中，独占 QSerialPort。
 *
 * 所有串口操作都通过“请求 -> 应答”的同步方式完成，耗时操作不会阻塞
 * GUI。取消升级用 requestCancel()，它只写一个原子标志，可以从 GUI 线程
 * 直接调用。
 */
class OtaWorker : public QObject
{
    Q_OBJECT

public:
    explicit OtaWorker(QObject *parent = nullptr);
    ~OtaWorker() override;

    // 线程安全：只设置取消标志
    void requestCancel();

public slots:
    void openPort(const QString &portName, int baudRate, int dataBits,
                  int stopBits, int parity);
    void closePort();
    void readVersion(int timeoutMs);
    void startUpgrade(const QByteArray &image, int chunkSize,
                      int responseTimeoutMs, int eraseTimeoutMs,
                      int endTimeoutMs, int frameDelayMs);

signals:
    void logMessage(const QString &message, int level);
    void portOpened(const QString &description);
    void portOpenFailed(const QString &error);
    void portClosed();
    void versionResult(bool ok, const QByteArray &payload, const QString &message);
    void progressChanged(int sent, int total, double kbps);
    void upgradeFinished(bool ok, bool cancelled, const QString &summary);
    void partitionChanged(const QString &partition);

private:
    bool ensurePortOpen(QString *error) const;
    bool request(quint8 cmd, const QByteArray &frame, int timeoutMs,
                 const QString &label, quint8 *status, QByteArray *data,
                 QString *error);
    bool readResponse(int timeoutMs, quint8 *cmd, quint8 *status,
                      QByteArray *data, QString *error);
    bool handshake(int timeoutMs, QString *error);
    bool queryVersion(int timeoutMs, QByteArray *payload, QString *error);
    void finishCancelled();
    void sleepInterruptible(int ms);
    void closePortInternal();

    QSerialPort *m_port = nullptr;
    std::atomic<bool> m_cancel;
};

#endif // OTAWORKER_H
