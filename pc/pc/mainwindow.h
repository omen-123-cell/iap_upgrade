#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QByteArray>
#include <QMainWindow>

#include "wifilistener.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class OtaWorker;
class QCloseEvent;
class QEvent;
class QThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    // 供命令行启动时预加载固件
    void loadFirmwareFile(const QString &path);

signals:
    void openPortRequested(const QString &portName, int baudRate, int dataBits,
                           int stopBits, int parity);
    void closePortRequested();
    void readVersionRequested(int timeoutMs);
    void startUpgradeRequested(const QByteArray &image, int chunkSize,
                               int responseTimeoutMs, int eraseTimeoutMs,
                               int endTimeoutMs, int frameDelayMs);

protected:
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void refreshSerialPorts();
    void togglePort();
    void onConnectionModeToggled(bool checked);

    void refreshLocalAddresses();
    void toggleWifiListening();
    void onWifiListeningStarted(quint16 port);
    void onWifiListeningFailed(const QString &error);
    void onWifiListeningStopped();
    void onWifiClientConnected(const QString &peer);
    void onWifiClientDisconnected(const QString &peer);
    void onWifiMessage(const WifiListener::Message &message);

    void onPortOpened(const QString &description);
    void onPortOpenFailed(const QString &error);
    void onPortClosed();
    void onVersionResult(bool ok, const QByteArray &payload, const QString &message);
    void onProgressChanged(int sent, int total, double kbps);
    void onUpgradeFinished(bool ok, bool cancelled, const QString &summary);
    void onPartitionChanged(const QString &partition);

    void selectFirmwareFile();
    void onFirmwarePathEdited();
    void startUpgrade();
    void stopUpgrade();
    void readDeviceVersion();

    void saveLog();
    void clearLog();
    void appendLog(const QString &message, int level);

private:
    bool collectPortSettings(QString *portName, int *baudRate, int *dataBits,
                             int *stopBits, int *parity, QString *error) const;
    void loadFirmware(const QString &path);
    void clearFirmware();
    void checkImageLayout();
    void updateUiState();
    void resetDeviceInfo();
    void loadSettings();
    void saveSettings();

    Ui::MainWindow *ui;
    QThread *m_workerThread = nullptr;
    OtaWorker *m_worker = nullptr;

    WifiListener *m_wifiListener = nullptr;
    bool m_wifiListening = false;
    int m_wifiRxMessages = 0;
    quint64 m_wifiRxBytes = 0;

    QByteArray m_rawImage;
    QByteArray m_sendImage;
    QString m_loadedPath;
    quint16 m_imageCrc = 0;

    bool m_portOpen = false;
    bool m_upgradeRunning = false;
    bool m_versionPending = false;
};

#endif // MAINWINDOW_H
