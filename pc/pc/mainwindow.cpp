#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "otaprotocol.h"
#include "otaworker.h"

#include <QCloseEvent>
#include <QDateTime>
#include <QDir>
#include <QEvent>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHostAddress>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSettings>
#include <QSignalBlocker>
#include <QStatusBar>
#include <QStringList>
#include <QTextStream>
#include <QThread>
#include <QTime>
#include <QtEndian>

namespace {

const quint32 kAppStart = 0x08010000u;
const quint32 kAppEnd = 0x08050000u;      // ADDR_RUN + SIZE_RUN

QString hex32(quint32 value)
{
    return QStringLiteral("0x%1")
        .arg(QString::number(value, 16).rightJustified(8, QLatin1Char('0')).toUpper());
}

} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ---------- 升级工作线程 ----------
    m_worker = new OtaWorker;
    m_workerThread = new QThread(this);
    m_worker->moveToThread(m_workerThread);
    connect(m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);

    connect(this, &MainWindow::openPortRequested,
            m_worker, &OtaWorker::openPort, Qt::QueuedConnection);
    connect(this, &MainWindow::closePortRequested,
            m_worker, &OtaWorker::closePort, Qt::QueuedConnection);
    connect(this, &MainWindow::readVersionRequested,
            m_worker, &OtaWorker::readVersion, Qt::QueuedConnection);
    connect(this, &MainWindow::startUpgradeRequested,
            m_worker, &OtaWorker::startUpgrade, Qt::QueuedConnection);

    connect(m_worker, &OtaWorker::logMessage, this, &MainWindow::appendLog);
    connect(m_worker, &OtaWorker::portOpened, this, &MainWindow::onPortOpened);
    connect(m_worker, &OtaWorker::portOpenFailed, this, &MainWindow::onPortOpenFailed);
    connect(m_worker, &OtaWorker::portClosed, this, &MainWindow::onPortClosed);
    connect(m_worker, &OtaWorker::versionResult, this, &MainWindow::onVersionResult);
    connect(m_worker, &OtaWorker::progressChanged, this, &MainWindow::onProgressChanged);
    connect(m_worker, &OtaWorker::upgradeFinished, this, &MainWindow::onUpgradeFinished);
    connect(m_worker, &OtaWorker::partitionChanged, this, &MainWindow::onPartitionChanged);

    m_workerThread->start();

    // ---------- WiFi 监听 ----------
    m_wifiListener = new WifiListener(this);
    connect(m_wifiListener, &WifiListener::listeningStarted,
            this, &MainWindow::onWifiListeningStarted);
    connect(m_wifiListener, &WifiListener::listeningFailed,
            this, &MainWindow::onWifiListeningFailed);
    connect(m_wifiListener, &WifiListener::listeningStopped,
            this, &MainWindow::onWifiListeningStopped);
    connect(m_wifiListener, &WifiListener::clientConnected,
            this, &MainWindow::onWifiClientConnected);
    connect(m_wifiListener, &WifiListener::clientDisconnected,
            this, &MainWindow::onWifiClientDisconnected);
    connect(m_wifiListener, &WifiListener::messageReceived,
            this, &MainWindow::onWifiMessage);
    connect(m_wifiListener, &WifiListener::logMessage,
            this, &MainWindow::appendLog);

    // ---------- 界面信号 ----------
    connect(ui->btnOpenPort, &QPushButton::clicked, this, &MainWindow::togglePort);
    connect(ui->btnRefreshPorts, &QPushButton::clicked, this, &MainWindow::refreshSerialPorts);
    connect(ui->rbSerial, &QRadioButton::toggled, this, &MainWindow::onConnectionModeToggled);
    connect(ui->rbEth, &QRadioButton::toggled, this, &MainWindow::onConnectionModeToggled);
    connect(ui->rbWifi, &QRadioButton::toggled, this, &MainWindow::onConnectionModeToggled);

    connect(ui->btnWifiListen, &QPushButton::clicked, this, &MainWindow::toggleWifiListening);
    connect(ui->btnRefreshIp, &QPushButton::clicked, this, &MainWindow::refreshLocalAddresses);

    connect(ui->btnSelectFile, &QPushButton::clicked, this, &MainWindow::selectFirmwareFile);
    connect(ui->leFirmwarePath, &QLineEdit::editingFinished,
            this, &MainWindow::onFirmwarePathEdited);
    connect(ui->btnStartUpgrade, &QPushButton::clicked, this, &MainWindow::startUpgrade);
    connect(ui->btnStopUpgrade, &QPushButton::clicked, this, &MainWindow::stopUpgrade);
    connect(ui->btnReadVersion, &QPushButton::clicked, this, &MainWindow::readDeviceVersion);

    connect(ui->btnClearLog, &QPushButton::clicked, this, &MainWindow::clearLog);
    connect(ui->btnSaveLog, &QPushButton::clicked, this, &MainWindow::saveLog);

    ui->cbSerialPort->installEventFilter(this);

    refreshSerialPorts();
    refreshLocalAddresses();
    loadSettings();
    updateUiState();

    appendLog(QStringLiteral("IAP 固件升级工具已启动。"), Ota::LogInfo);
    appendLog(QStringLiteral("请选择串口并打开，复位设备进入 Bootloader 后即可开始升级。"),
              Ota::LogInfo);
}

MainWindow::~MainWindow()
{
    if (m_wifiListener)
        m_wifiListener->stop();

    if (m_workerThread && m_workerThread->isRunning()) {
        m_worker->requestCancel();
        m_workerThread->quit();
        if (!m_workerThread->wait(6000)) {
            m_workerThread->terminate();
            m_workerThread->wait(1000);
        }
    }
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_upgradeRunning) {
        const QMessageBox::StandardButton answer = QMessageBox::question(
            this, QStringLiteral("退出"),
            QStringLiteral("升级正在进行，确定要退出吗？\n"
                           "退出会中断升级，设备需要复位后才能重新升级。"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (answer != QMessageBox::Yes) {
            event->ignore();
            return;
        }
        m_worker->requestCancel();
    }

    if (m_wifiListener)
        m_wifiListener->stop();

    saveSettings();

    if (m_workerThread->isRunning()) {
        m_workerThread->quit();
        if (!m_workerThread->wait(6000)) {
            m_workerThread->terminate();
            m_workerThread->wait(1000);
        }
    }

    event->accept();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->cbSerialPort && event->type() == QEvent::MouseButtonPress
        && !m_portOpen && !m_upgradeRunning) {
        refreshSerialPorts();
    }
    return QMainWindow::eventFilter(watched, event);
}

// ---------------- 串口 ----------------

void MainWindow::refreshSerialPorts()
{
    const QString current = ui->cbSerialPort->currentText();

    QSignalBlocker blocker(ui->cbSerialPort);
    ui->cbSerialPort->clear();

    const QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : ports) {
        ui->cbSerialPort->addItem(info.portName());
        const int index = ui->cbSerialPort->count() - 1;

        QStringList details;
        if (!info.description().isEmpty())
            details << info.description();
        if (!info.manufacturer().isEmpty())
            details << info.manufacturer();
        if (!details.isEmpty())
            ui->cbSerialPort->setItemData(index, details.join(QStringLiteral(" | ")),
                                          Qt::ToolTipRole);
    }

    if (!current.isEmpty())
        ui->cbSerialPort->setCurrentText(current);
    else if (ui->cbSerialPort->count() > 0)
        ui->cbSerialPort->setCurrentIndex(0);
}

bool MainWindow::collectPortSettings(QString *portName, int *baudRate, int *dataBits,
                                     int *stopBits, int *parity, QString *error) const
{
    const QString name = ui->cbSerialPort->currentText().trimmed();
    if (name.isEmpty()) {
        *error = QStringLiteral("请选择或输入串口号。");
        return false;
    }

    bool ok = false;
    const int baud = ui->cbBaudRate->currentText().trimmed().toInt(&ok);
    if (!ok || baud <= 0) {
        *error = QStringLiteral("波特率无效：%1").arg(ui->cbBaudRate->currentText());
        return false;
    }

    *portName = name;
    *baudRate = baud;

    switch (ui->cbDataBits->currentIndex()) {
    case 0: *dataBits = QSerialPort::Data5; break;
    case 1: *dataBits = QSerialPort::Data6; break;
    case 2: *dataBits = QSerialPort::Data7; break;
    default: *dataBits = QSerialPort::Data8; break;
    }

    switch (ui->cbStopBits->currentIndex()) {
    case 1: *stopBits = QSerialPort::OneAndHalfStop; break;
    case 2: *stopBits = QSerialPort::TwoStop; break;
    default: *stopBits = QSerialPort::OneStop; break;
    }

    switch (ui->cbParity->currentIndex()) {
    case 1: *parity = QSerialPort::OddParity; break;
    case 2: *parity = QSerialPort::EvenParity; break;
    default: *parity = QSerialPort::NoParity; break;
    }
    return true;
}

void MainWindow::togglePort()
{
    if (m_upgradeRunning)
        return;

    if (m_portOpen) {
        ui->btnOpenPort->setEnabled(false);
        emit closePortRequested();
        return;
    }

    if (!ui->rbSerial->isChecked()) {
        appendLog(QStringLiteral("串口未在选择状态；WiFi 模式请点击“开始监听”。"),
                  Ota::LogWarning);
        return;
    }

    QString portName;
    QString error;
    int baud = 0;
    int dataBits = 0;
    int stopBits = 0;
    int parity = 0;
    if (!collectPortSettings(&portName, &baud, &dataBits, &stopBits, &parity, &error)) {
        appendLog(error, Ota::LogError);
        return;
    }

    appendLog(QStringLiteral("正在打开串口 %1 @ %2 ...").arg(portName).arg(baud), Ota::LogInfo);
    emit openPortRequested(portName, baud, dataBits, stopBits, parity);
    ui->btnOpenPort->setEnabled(false);
}

void MainWindow::onConnectionModeToggled(bool checked)
{
    if (!checked)
        return;

    if (ui->rbWifi->isChecked()) {
        refreshLocalAddresses();
        appendLog(QStringLiteral("WiFi 模式：PC 作为 TCP 服务器，ESP32 连接后"
                                 "收到的数据会打印到日志。"),
                  Ota::LogInfo);
    } else if (ui->rbEth->isChecked()) {
        appendLog(QStringLiteral("以太网通道暂未实现，请选择串口或 WiFi。"),
                  Ota::LogWarning);
    }
    updateUiState();
}

void MainWindow::onPortOpened(const QString &description)
{
    m_portOpen = true;
    appendLog(QStringLiteral("串口已打开：%1").arg(description), Ota::LogSuccess);
    statusBar()->showMessage(QStringLiteral("串口已打开：%1").arg(description), 3000);
    updateUiState();
}

void MainWindow::onPortOpenFailed(const QString &error)
{
    m_portOpen = false;
    appendLog(QStringLiteral("打开串口失败：%1").arg(error), Ota::LogError);

    const QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    QStringList names;
    for (const QSerialPortInfo &info : ports)
        names << info.portName();
    if (!names.isEmpty())
        appendLog(QStringLiteral("当前可用串口：%1").arg(names.join(QStringLiteral(", "))),
                  Ota::LogInfo);

    updateUiState();
}

void MainWindow::onPortClosed()
{
    m_portOpen = false;
    m_versionPending = false;
    resetDeviceInfo();
    appendLog(QStringLiteral("串口已关闭。"), Ota::LogInfo);
    statusBar()->showMessage(QStringLiteral("就绪"), 2000);
    updateUiState();
}

void MainWindow::resetDeviceInfo()
{
    ui->leFwVersion->clear();
    ui->lePartition->clear();
}

// ---------------- WiFi 监听 ----------------

void MainWindow::refreshLocalAddresses()
{
    QStringList addresses;
    const QList<QHostAddress> all = QNetworkInterface::allAddresses();
    for (const QHostAddress &address : all) {
        if (address.protocol() != QAbstractSocket::IPv4Protocol || address.isLoopback())
            continue;
        const QString text = address.toString();
        if (!addresses.contains(text))
            addresses.append(text);
    }

    // 优先展示常见的局域网地址，避免把虚拟网卡当成主地址
    QString primary;
    int bestScore = -1;
    for (const QString &address : addresses) {
        int score = 0;
        if (address.startsWith(QStringLiteral("192.168.")))
            score = 3;
        else if (address.startsWith(QStringLiteral("10.")))
            score = 2;
        else if (address.startsWith(QStringLiteral("172.")))
            score = 1;
        if (score > bestScore) {
            bestScore = score;
            primary = address;
        }
    }

    ui->leLocalIp->setText(primary);
    ui->leLocalIp->setToolTip(
        addresses.isEmpty()
            ? QStringLiteral("未找到本机 IPv4 地址，请检查网络连接。")
            : QStringLiteral("本机 IPv4 地址：\n%1").arg(addresses.join(QLatin1Char('\n'))));
}

void MainWindow::toggleWifiListening()
{
    if (m_wifiListening) {
        m_wifiListener->stop();
        return;
    }

    if (m_upgradeRunning)
        return;

    if (!ui->rbWifi->isChecked()) {
        appendLog(QStringLiteral("请先在“连接方式”中选择 wifi。"), Ota::LogWarning);
        return;
    }

    refreshLocalAddresses();
    const quint16 port = static_cast<quint16>(ui->sbListenPort->value());
    appendLog(QStringLiteral("正在监听 TCP 端口 %1 ...").arg(port), Ota::LogInfo);

    m_wifiListener->start(port);
    // 结果由 listeningStarted / listeningFailed 信号统一处理
}

void MainWindow::onWifiListeningStarted(quint16 port)
{
    m_wifiListening = true;
    m_wifiRxMessages = 0;
    m_wifiRxBytes = 0;

    ui->lblWifiStatus->setText(QStringLiteral("监听中（端口 %1）").arg(port));
    ui->lblWifiStatus->setStyleSheet(
        QStringLiteral("color: rgb(0, 140, 0); font-weight: bold;"));
    ui->lblWifiClient->setText(QStringLiteral("未连接"));
    ui->lblWifiClient->setStyleSheet(QString());
    ui->lblWifiStats->setText(QStringLiteral("0 条 / 0 字节"));

    appendLog(QStringLiteral("WiFi 监听已启动：TCP 端口 %1，等待 ESP32 连接。").arg(port),
              Ota::LogSuccess);

    const QString localIp = ui->leLocalIp->text().trimmed();
    if (localIp.isEmpty()) {
        appendLog(QStringLiteral("未找到本机 IPv4 地址，请检查网络连接后点击“刷新”。"),
                  Ota::LogWarning);
    } else {
        appendLog(QStringLiteral("请确认 ESP32 固件中 SERVER_IP = \"%1\"、"
                                 "SERVER_PORT = %2。")
                      .arg(localIp)
                      .arg(port),
                  Ota::LogInfo);
    }
    appendLog(QStringLiteral("若 ESP32 连不上，请检查 Windows 防火墙是否放行本程序。"),
              Ota::LogInfo);

    statusBar()->showMessage(QStringLiteral("WiFi 监听中：端口 %1").arg(port), 3000);
    updateUiState();
}

void MainWindow::onWifiListeningFailed(const QString &error)
{
    m_wifiListening = false;
    ui->lblWifiStatus->setText(QStringLiteral("监听失败"));
    ui->lblWifiStatus->setStyleSheet(
        QStringLiteral("color: rgb(198, 40, 40); font-weight: bold;"));

    appendLog(QStringLiteral("WiFi 监听失败：%1").arg(error), Ota::LogError);
    appendLog(QStringLiteral("请检查端口是否被其他程序占用，并在 Windows 防火墙中"
                             "允许本程序通过。"),
              Ota::LogWarning);
    updateUiState();
}

void MainWindow::onWifiListeningStopped()
{
    m_wifiListening = false;
    ui->lblWifiStatus->setText(QStringLiteral("未监听"));
    ui->lblWifiStatus->setStyleSheet(
        QStringLiteral("color: rgb(120, 120, 120);"));
    ui->lblWifiClient->setText(QStringLiteral("未连接"));
    ui->lblWifiClient->setStyleSheet(QString());

    appendLog(QStringLiteral("WiFi 监听已停止。"), Ota::LogInfo);
    statusBar()->showMessage(QStringLiteral("WiFi 监听已停止"), 3000);
    updateUiState();
}

void MainWindow::onWifiClientConnected(const QString &peer)
{
    ui->lblWifiClient->setText(peer);
    ui->lblWifiClient->setStyleSheet(
        QStringLiteral("color: rgb(0, 140, 0); font-weight: bold;"));
    appendLog(QStringLiteral("ESP32 已连接：%1").arg(peer), Ota::LogSuccess);
    statusBar()->showMessage(QStringLiteral("ESP32 已连接：%1").arg(peer), 3000);
}

void MainWindow::onWifiClientDisconnected(const QString &peer)
{
    ui->lblWifiClient->setText(QStringLiteral("未连接"));
    ui->lblWifiClient->setStyleSheet(QString());
    appendLog(QStringLiteral("客户端已断开：%1").arg(peer), Ota::LogWarning);
    if (m_wifiListening)
        appendLog(QStringLiteral("继续监听，等待 ESP32 重连 ..."), Ota::LogInfo);
}

void MainWindow::onWifiMessage(const WifiListener::Message &message)
{
    ++m_wifiRxMessages;
    m_wifiRxBytes += static_cast<quint64>(message.rawLine.toUtf8().size());
    ui->lblWifiStats->setText(QStringLiteral("%1 条 / %2 字节")
                                  .arg(m_wifiRxMessages)
                                  .arg(m_wifiRxBytes));

    if (!message.parsed) {
        appendLog(QStringLiteral("[WiFi] %1 收到无法解析的数据：%2")
                      .arg(message.peer, message.rawLine),
                  Ota::LogWarning);
        return;
    }

    appendLog(QStringLiteral("[WiFi] %1 | id=%2 len=%3 | text=\"%4\"")
                  .arg(message.peer)
                  .arg(message.id)
                  .arg(message.len)
                  .arg(message.text),
              Ota::LogInfo);

    // ESP32 的 len 是字节数；中文文本按 UTF-8 编码后字节数会大于字符数
    const int actualBytes = message.text.toUtf8().size();
    if (static_cast<quint32>(actualBytes) != message.len) {
        appendLog(QStringLiteral("[WiFi] 注意：len=%1，文本实际 UTF-8 字节数为 %2。")
                      .arg(message.len)
                      .arg(actualBytes),
                  Ota::LogWarning);
    }
}

// ---------------- 版本 / 升级 ----------------

void MainWindow::readDeviceVersion()
{
    if (!m_portOpen || m_upgradeRunning || m_versionPending)
        return;

    m_versionPending = true;
    const int timeoutMs = qRound(ui->sbResponseTimeout->value() * 1000.0);

    appendLog(QStringLiteral("读取设备版本 ..."), Ota::LogInfo);
    emit readVersionRequested(timeoutMs);
    updateUiState();
}

void MainWindow::onVersionResult(bool ok, const QByteArray &payload, const QString &message)
{
    m_versionPending = false;

    if (ok) {
        const QString version = Ota::versionString(payload);
        ui->leFwVersion->setText(version);
        ui->lePartition->setText(QStringLiteral("Bootloader"));
        appendLog(QStringLiteral("设备版本：%1（原始 %2）")
                      .arg(version, Ota::hexString(payload)),
                  Ota::LogSuccess);
        statusBar()->showMessage(QStringLiteral("版本读取成功"), 3000);
    } else {
        appendLog(QStringLiteral("读取版本失败：%1").arg(message), Ota::LogError);
        appendLog(QStringLiteral("提示：请复位设备进入 Bootloader 后重试。"), Ota::LogWarning);
    }

    updateUiState();
}

void MainWindow::onPartitionChanged(const QString &partition)
{
    ui->lePartition->setText(partition);
}

void MainWindow::startUpgrade()
{
    if (m_upgradeRunning)
        return;

    if (!m_portOpen) {
        appendLog(QStringLiteral("请先打开串口。"), Ota::LogError);
        return;
    }
    if (!ui->rbSerial->isChecked()) {
        appendLog(QStringLiteral("当前固件仅支持串口升级。"), Ota::LogError);
        return;
    }
    if (m_sendImage.isEmpty()) {
        appendLog(QStringLiteral("请先选择有效的固件文件。"), Ota::LogError);
        return;
    }

    const int chunkSize = ui->sbChunkSize->value();
    if (chunkSize < 4 || chunkSize > 250 || (chunkSize % 4) != 0) {
        appendLog(QStringLiteral("每帧数据长度必须是 4~250 之间且为 4 的倍数。"), Ota::LogError);
        return;
    }

    const int responseTimeoutMs = qRound(ui->sbResponseTimeout->value() * 1000.0);
    const int eraseTimeoutMs = qRound(ui->sbEraseTimeout->value() * 1000.0);
    const int endTimeoutMs = qRound(ui->sbEndTimeout->value() * 1000.0);
    const int frameDelayMs = ui->sbFrameDelay->value();

    m_upgradeRunning = true;
    m_versionPending = false;

    ui->progressBar->setValue(0);
    ui->progressBar->setFormat(QStringLiteral("0 / %1 字节").arg(m_sendImage.size()));
    ui->lePartition->setText(QStringLiteral("Bootloader"));
    updateUiState();

    appendLog(QStringLiteral("================ IAP 升级开始 ================"),
              Ota::LogInfo);

    emit startUpgradeRequested(m_sendImage, chunkSize, responseTimeoutMs,
                               eraseTimeoutMs, endTimeoutMs, frameDelayMs);
}

void MainWindow::stopUpgrade()
{
    if (!m_upgradeRunning)
        return;

    appendLog(QStringLiteral("正在停止升级，等待当前帧结束 ..."), Ota::LogWarning);
    ui->btnStopUpgrade->setEnabled(false);
    m_worker->requestCancel();
}

void MainWindow::onProgressChanged(int sent, int total, double kbps)
{
    if (total <= 0)
        return;

    const int percent = static_cast<int>(qint64(sent) * 100 / total);
    ui->progressBar->setValue(qBound(0, percent, 100));
    ui->progressBar->setFormat(QStringLiteral("%1 / %2 字节   %3 KB/s")
                                   .arg(sent)
                                   .arg(total)
                                   .arg(kbps, 0, 'f', 2));
    statusBar()->showMessage(QStringLiteral("升级中 ... %1 / %2 字节")
                                 .arg(sent)
                                 .arg(total));
}

void MainWindow::onUpgradeFinished(bool ok, bool cancelled, const QString &summary)
{
    m_upgradeRunning = false;
    updateUiState();

    if (ok) {
        ui->progressBar->setValue(100);
        ui->progressBar->setFormat(QStringLiteral("100%"));
        appendLog(summary, Ota::LogSuccess);
        statusBar()->showMessage(QStringLiteral("升级完成"), 5000);
    } else if (cancelled) {
        appendLog(summary, Ota::LogWarning);
        statusBar()->showMessage(QStringLiteral("升级已取消"), 5000);
    } else {
        appendLog(summary, Ota::LogError);
        statusBar()->showMessage(QStringLiteral("升级失败"), 5000);
    }
}

// ---------------- 固件文件 ----------------

void MainWindow::selectFirmwareFile()
{
    QSettings settings;
    QString startDir = settings.value(QStringLiteral("firmware/lastDir"),
                                      QDir::homePath()).toString();
    const QString currentPath = ui->leFirmwarePath->text().trimmed();
    if (!currentPath.isEmpty())
        startDir = QFileInfo(currentPath).absolutePath();

    const QString path = QFileDialog::getOpenFileName(
        this, QStringLiteral("选择固件文件"), startDir,
        QStringLiteral("固件文件 (*.bin);;所有文件 (*)"));
    if (path.isEmpty())
        return;

    settings.setValue(QStringLiteral("firmware/lastDir"), QFileInfo(path).absolutePath());
    loadFirmware(path);
}

void MainWindow::onFirmwarePathEdited()
{
    const QString path = ui->leFirmwarePath->text().trimmed();
    if (path.isEmpty()) {
        clearFirmware();
        return;
    }
    if (path == m_loadedPath)
        return;
    loadFirmware(path);
}

void MainWindow::loadFirmwareFile(const QString &path)
{
    if (!path.trimmed().isEmpty())
        loadFirmware(path.trimmed());
}

void MainWindow::loadFirmware(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        appendLog(QStringLiteral("打不开固件文件：%1（%2）").arg(path, file.errorString()),
                  Ota::LogError);
        clearFirmware();
        return;
    }

    const QByteArray raw = file.readAll();
    file.close();

    if (raw.isEmpty()) {
        appendLog(QStringLiteral("固件文件是空的：%1").arg(path), Ota::LogError);
        clearFirmware();
        return;
    }

    const int pad = (4 - (raw.size() % 4)) % 4;
    QByteArray image = raw;
    if (pad > 0)
        image.append(QByteArray(pad, static_cast<char>(0xFF)));

    if (image.size() > 0xFFFF) {
        appendLog(QStringLiteral("固件过大：原始 %1 字节，补齐后 %2 字节，"
                                 "超过固件端 uint16_t 计数上限 65535 字节。")
                      .arg(raw.size())
                      .arg(image.size()),
                  Ota::LogError);
        clearFirmware();
        return;
    }

    m_rawImage = raw;
    m_sendImage = image;
    m_loadedPath = QFileInfo(path).absoluteFilePath();
    m_imageCrc = Ota::crc16Modbus(image);

    ui->leFirmwarePath->setText(m_loadedPath);
    ui->leFileSize->setText(
        pad > 0 ? QStringLiteral("%1 字节（补 0xFF 后 %2）").arg(raw.size()).arg(image.size())
                : QStringLiteral("%1 字节").arg(raw.size()));
    ui->leCrc16->setText(Ota::crcHex(m_imageCrc));

    appendLog(QStringLiteral("已加载固件：%1").arg(m_loadedPath), Ota::LogInfo);
    appendLog(QStringLiteral("原始大小 %1 字节；发送大小 %2 字节%3。")
                  .arg(raw.size())
                  .arg(image.size())
                  .arg(pad > 0
                           ? QStringLiteral("（末尾补 %1 个 0xFF）").arg(pad)
                           : QString()),
              Ota::LogInfo);
    appendLog(QStringLiteral("目标 CRC16 = %1（按发送内容计算）。")
                  .arg(Ota::crcHex(m_imageCrc)),
              Ota::LogInfo);

    int chunkSize = qMax(4, ui->sbChunkSize->value());
    int baud = ui->cbBaudRate->currentText().toInt();
    if (baud <= 0)
        baud = 9600;
    const int frames = (image.size() + chunkSize - 1) / chunkSize;
    const double estimated = frames * (chunkSize + 13) * 10.0 / baud;
    appendLog(QStringLiteral("分包：%1 帧 x %2 字节，预计耗时约 %3 秒（不含擦除）。")
                  .arg(frames)
                  .arg(chunkSize)
                  .arg(estimated, 0, 'f', 1),
              Ota::LogInfo);

    checkImageLayout();
    updateUiState();
}

void MainWindow::clearFirmware()
{
    m_rawImage.clear();
    m_sendImage.clear();
    m_loadedPath.clear();
    m_imageCrc = 0;

    ui->leFirmwarePath->clear();
    ui->leFileSize->clear();
    ui->leCrc16->clear();
    updateUiState();
}

void MainWindow::checkImageLayout()
{
    if (m_rawImage.size() < 8)
        return;

    const quint8 *data = reinterpret_cast<const quint8 *>(m_rawImage.constData());
    const quint32 initialSp = qFromLittleEndian<quint32>(data);
    const quint32 resetVector = qFromLittleEndian<quint32>(data + 4);

    bool ok = true;

    if (resetVector < kAppStart || resetVector >= kAppEnd) {
        appendLog(QStringLiteral("[提示] 复位向量 = %1，不在 App 区 0x08010000~0x0804FFFF。"
                                 "升级和 CRC 校验会成功，但跳转后不会运行这个 App。"
                                 "请把 App 工程 IROM1 起始地址改为 0x08010000"
                                 "（大小 0x00040000）后重新编译。")
                      .arg(hex32(resetVector)),
                  Ota::LogWarning);
        ok = false;
    }

    if ((initialSp & 0x2FFE0000u) != 0x20000000u) {
        appendLog(QStringLiteral("[提示] 初始栈指针 = %1，不满足 Jump_To_RunArea() 的检查，"
                                 "可能不会跳转。")
                      .arg(hex32(initialSp)),
                  Ota::LogWarning);
        ok = false;
    }

    if (ok)
        appendLog(QStringLiteral("[检查] 向量表位于 App 区，链接地址正确。"), Ota::LogSuccess);
}

// ---------------- 界面状态 ----------------

void MainWindow::updateUiState()
{
    const bool busy = m_upgradeRunning;
    const bool serialMode = ui->rbSerial->isChecked();
    const bool wifiMode = ui->rbWifi->isChecked();
    const bool canEditConnection = !m_portOpen && !busy && !m_wifiListening;

    ui->rbSerial->setEnabled(canEditConnection);
    ui->rbEth->setEnabled(canEditConnection);
    ui->rbWifi->setEnabled(canEditConnection);

    ui->cbSerialPort->setEnabled(canEditConnection && serialMode);
    ui->cbBaudRate->setEnabled(canEditConnection && serialMode);
    ui->cbDataBits->setEnabled(canEditConnection && serialMode);
    ui->cbStopBits->setEnabled(canEditConnection && serialMode);
    ui->cbParity->setEnabled(canEditConnection && serialMode);
    ui->btnRefreshPorts->setEnabled(canEditConnection && serialMode);

    ui->gbNetwork->setEnabled(wifiMode && !busy);
    ui->sbListenPort->setEnabled(wifiMode && !busy && !m_wifiListening);
    ui->btnRefreshIp->setEnabled(wifiMode && !busy && !m_wifiListening);
    ui->btnWifiListen->setEnabled(wifiMode && !busy);
    ui->btnWifiListen->setText(m_wifiListening ? QStringLiteral("停止监听")
                                               : QStringLiteral("开始监听"));
    ui->btnWifiListen->setStyleSheet(
        m_wifiListening
            ? QStringLiteral("QPushButton{background-color:rgb(220,40,40);color:white;}"
                             "QPushButton:disabled{background-color:rgb(200,200,200);"
                             "color:rgb(120,120,120);}")
            : QStringLiteral("QPushButton{background-color:rgb(0,200,0);color:white;}"
                             "QPushButton:disabled{background-color:rgb(200,200,200);"
                             "color:rgb(120,120,120);}"));

    ui->btnOpenPort->setEnabled(serialMode && !busy);
    ui->btnOpenPort->setText(m_portOpen ? QStringLiteral("关闭串口")
                                        : QStringLiteral("打开串口"));
    ui->btnOpenPort->setStyleSheet(
        m_portOpen
            ? QStringLiteral("QPushButton{background-color:rgb(220,40,40);color:white;}"
                             "QPushButton:disabled{background-color:rgb(200,200,200);"
                             "color:rgb(120,120,120);}")
            : QStringLiteral("QPushButton{background-color:rgb(0,200,0);color:white;}"
                             "QPushButton:disabled{background-color:rgb(200,200,200);"
                             "color:rgb(120,120,120);}"));
    ui->btnOpenPort->setToolTip(
        serialMode ? QString()
                   : QStringLiteral("WiFi 模式请使用“开始监听”按钮。"));

    ui->btnSelectFile->setEnabled(!busy);
    ui->leFirmwarePath->setEnabled(!busy);

    ui->btnStartUpgrade->setEnabled(!busy && m_portOpen && !m_versionPending
                                    && serialMode && !m_sendImage.isEmpty());
    ui->btnStopUpgrade->setEnabled(busy);
    ui->btnReadVersion->setEnabled(m_portOpen && !busy && !m_versionPending && serialMode);

    ui->sbChunkSize->setEnabled(!busy);
    ui->sbResponseTimeout->setEnabled(!busy);
    ui->sbEraseTimeout->setEnabled(!busy);
    ui->sbEndTimeout->setEnabled(!busy);
    ui->sbFrameDelay->setEnabled(!busy);
}

// ---------------- 日志 ----------------

void MainWindow::appendLog(const QString &message, int level)
{
    QString color;
    switch (level) {
    case Ota::LogSuccess: color = QStringLiteral("#1b7f3a"); break;
    case Ota::LogWarning: color = QStringLiteral("#b26a00"); break;
    case Ota::LogError:   color = QStringLiteral("#c62828"); break;
    default:              color = QStringLiteral("#202020"); break;
    }

    QString body = message.toHtmlEscaped();
    body.replace(QLatin1Char('\n'), QStringLiteral("<br/>"));

    const QString time = QTime::currentTime().toString(QStringLiteral("HH:mm:ss.zzz"));
    ui->textLog->append(QStringLiteral("<span style=\"color:#909090;\">[%1]</span> "
                                       "<span style=\"color:%2;\">%3</span>")
                            .arg(time, color, body));
}

void MainWindow::clearLog()
{
    ui->textLog->clear();
}

void MainWindow::saveLog()
{
    const QString defaultPath = QDir(QDir::homePath())
        .filePath(QStringLiteral("iap_log_%1.txt")
                      .arg(QDateTime::currentDateTime().toString(
                          QStringLiteral("yyyyMMdd_HHmmss"))));

    const QString path = QFileDialog::getSaveFileName(
        this, QStringLiteral("保存日志"), defaultPath,
        QStringLiteral("文本文件 (*.txt);;所有文件 (*)"));
    if (path.isEmpty())
        return;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        appendLog(QStringLiteral("保存日志失败：%1（%2）").arg(path, file.errorString()),
                  Ota::LogError);
        return;
    }

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << ui->textLog->toPlainText();
    stream.flush();
    file.close();

    appendLog(QStringLiteral("日志已保存：%1").arg(path), Ota::LogSuccess);
}

// ---------------- 配置持久化 ----------------

void MainWindow::loadSettings()
{
    QSettings settings;

    const QString port = settings.value(QStringLiteral("serial/port")).toString();
    if (!port.isEmpty())
        ui->cbSerialPort->setCurrentText(port);

    ui->cbBaudRate->setCurrentText(
        settings.value(QStringLiteral("serial/baud"), QStringLiteral("9600")).toString());
    ui->cbDataBits->setCurrentIndex(
        qBound(0, settings.value(QStringLiteral("serial/dataBits"), 3).toInt(), 3));
    ui->cbStopBits->setCurrentIndex(
        qBound(0, settings.value(QStringLiteral("serial/stopBits"), 0).toInt(), 2));
    ui->cbParity->setCurrentIndex(
        qBound(0, settings.value(QStringLiteral("serial/parity"), 0).toInt(), 2));

    ui->sbListenPort->setValue(
        qBound(1, settings.value(QStringLiteral("wifi/listenPort"), 8080).toInt(), 65535));

    const QString connectionMode =
        settings.value(QStringLiteral("connection/mode"), QStringLiteral("serial")).toString();
    if (connectionMode == QLatin1String("wifi"))
        ui->rbWifi->setChecked(true);
    else if (connectionMode == QLatin1String("eth"))
        ui->rbEth->setChecked(true);
    else
        ui->rbSerial->setChecked(true);

    int chunk = settings.value(QStringLiteral("transfer/chunk"), 128).toInt();
    chunk = qBound(4, chunk, 250);
    chunk -= chunk % 4;
    ui->sbChunkSize->setValue(qMax(4, chunk));

    ui->sbResponseTimeout->setValue(
        settings.value(QStringLiteral("transfer/responseTimeout"), 2.0).toDouble());
    ui->sbEraseTimeout->setValue(
        settings.value(QStringLiteral("transfer/eraseTimeout"), 15.0).toDouble());
    ui->sbEndTimeout->setValue(
        settings.value(QStringLiteral("transfer/endTimeout"), 5.0).toDouble());
    ui->sbFrameDelay->setValue(
        settings.value(QStringLiteral("transfer/frameDelay"), 0).toInt());

    const QString firmwarePath = settings.value(QStringLiteral("firmware/lastFile")).toString();
    if (!firmwarePath.isEmpty() && QFileInfo::exists(firmwarePath))
        loadFirmware(firmwarePath);

    const QByteArray geometry = settings.value(QStringLiteral("window/geometry")).toByteArray();
    if (!geometry.isEmpty())
        restoreGeometry(geometry);
}

void MainWindow::saveSettings()
{
    QSettings settings;

    settings.setValue(QStringLiteral("serial/port"), ui->cbSerialPort->currentText());
    settings.setValue(QStringLiteral("serial/baud"), ui->cbBaudRate->currentText());
    settings.setValue(QStringLiteral("serial/dataBits"), ui->cbDataBits->currentIndex());
    settings.setValue(QStringLiteral("serial/stopBits"), ui->cbStopBits->currentIndex());
    settings.setValue(QStringLiteral("serial/parity"), ui->cbParity->currentIndex());

    settings.setValue(QStringLiteral("wifi/listenPort"), ui->sbListenPort->value());
    settings.setValue(QStringLiteral("connection/mode"),
                      ui->rbWifi->isChecked()
                          ? QStringLiteral("wifi")
                          : (ui->rbEth->isChecked() ? QStringLiteral("eth")
                                                    : QStringLiteral("serial")));

    settings.setValue(QStringLiteral("transfer/chunk"), ui->sbChunkSize->value());
    settings.setValue(QStringLiteral("transfer/responseTimeout"),
                      ui->sbResponseTimeout->value());
    settings.setValue(QStringLiteral("transfer/eraseTimeout"), ui->sbEraseTimeout->value());
    settings.setValue(QStringLiteral("transfer/endTimeout"), ui->sbEndTimeout->value());
    settings.setValue(QStringLiteral("transfer/frameDelay"), ui->sbFrameDelay->value());

    if (!m_loadedPath.isEmpty())
        settings.setValue(QStringLiteral("firmware/lastFile"), m_loadedPath);

    settings.setValue(QStringLiteral("window/geometry"), saveGeometry());
}
