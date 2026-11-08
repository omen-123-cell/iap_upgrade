/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *centralLayout;
    QTabWidget *tabWidget;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *gbConnectstyle;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *rbSerial;
    QRadioButton *rbEth;
    QRadioButton *rbWifi;
    QSpacerItem *horizontalSpacer_5;
    QGroupBox *gbSerial;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_9;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QComboBox *cbSerialPort;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QComboBox *cbBaudRate;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QComboBox *cbDataBits;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_10;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    QComboBox *cbStopBits;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QComboBox *cbParity;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnOpenPort;
    QPushButton *btnRefreshPorts;
    QGroupBox *gbNetwork;
    QVBoxLayout *verticalLayout_wifi;
    QHBoxLayout *horizontalLayout_wifiSettings;
    QLabel *label_8;
    QLineEdit *leLocalIp;
    QPushButton *btnRefreshIp;
    QLabel *label_9;
    QSpinBox *sbListenPort;
    QPushButton *btnWifiListen;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_wifiStatus;
    QLabel *label_10;
    QLabel *lblWifiStatus;
    QLabel *label_15;
    QLabel *lblWifiClient;
    QLabel *label_16;
    QLabel *lblWifiStats;
    QSpacerItem *horizontalSpacer_8;
    QGroupBox *gbFirmware;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_12;
    QLineEdit *leFirmwarePath;
    QPushButton *btnSelectFile;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_13;
    QLineEdit *leFileSize;
    QLabel *label_14;
    QLineEdit *leCrc16;
    QSpacerItem *horizontalSpacer_4;
    QGroupBox *gbUpgradeProgress;
    QHBoxLayout *horizontalLayout_17;
    QProgressBar *progressBar;
    QPushButton *btnStartUpgrade;
    QPushButton *btnStopUpgrade;
    QGroupBox *gbDevice;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *leFwVersion;
    QLabel *label_2;
    QLineEdit *lePartition;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *btnReadVersion;
    QGroupBox *gbLog;
    QVBoxLayout *verticalLayout;
    QTextEdit *textLog;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnClearLog;
    QPushButton *btnSaveLog;
    QSpacerItem *horizontalSpacer_7;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *gbTransferParams;
    QFormLayout *formLayout;
    QLabel *labelChunkSize;
    QSpinBox *sbChunkSize;
    QLabel *labelResponseTimeout;
    QDoubleSpinBox *sbResponseTimeout;
    QLabel *labelEraseTimeout;
    QDoubleSpinBox *sbEraseTimeout;
    QLabel *labelEndTimeout;
    QDoubleSpinBox *sbEndTimeout;
    QLabel *labelFrameDelay;
    QSpinBox *sbFrameDelay;
    QGroupBox *gbProtocolInfo;
    QVBoxLayout *verticalLayout_6;
    QLabel *labelProtocolInfo;
    QLabel *labelTransferNote;
    QSpacerItem *verticalSpacer_3;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QButtonGroup *buttonGroup;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 800);
        MainWindow->setMinimumSize(QSize(700, 620));
        MainWindow->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 3px solid #333333;\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralLayout = new QVBoxLayout(centralwidget);
        centralLayout->setSpacing(0);
        centralLayout->setObjectName(QString::fromUtf8("centralLayout"));
        centralLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(10, 10, 10, 10);
        gbConnectstyle = new QGroupBox(tab_2);
        gbConnectstyle->setObjectName(QString::fromUtf8("gbConnectstyle"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gbConnectstyle->sizePolicy().hasHeightForWidth());
        gbConnectstyle->setSizePolicy(sizePolicy);
        horizontalLayout_3 = new QHBoxLayout(gbConnectstyle);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(40, -1, -1, -1);
        rbSerial = new QRadioButton(gbConnectstyle);
        buttonGroup = new QButtonGroup(MainWindow);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(rbSerial);
        rbSerial->setObjectName(QString::fromUtf8("rbSerial"));
        rbSerial->setChecked(true);

        horizontalLayout_3->addWidget(rbSerial);

        rbEth = new QRadioButton(gbConnectstyle);
        buttonGroup->addButton(rbEth);
        rbEth->setObjectName(QString::fromUtf8("rbEth"));

        horizontalLayout_3->addWidget(rbEth);

        rbWifi = new QRadioButton(gbConnectstyle);
        buttonGroup->addButton(rbWifi);
        rbWifi->setObjectName(QString::fromUtf8("rbWifi"));

        horizontalLayout_3->addWidget(rbWifi);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);


        verticalLayout_2->addWidget(gbConnectstyle);

        gbSerial = new QGroupBox(tab_2);
        gbSerial->setObjectName(QString::fromUtf8("gbSerial"));
        sizePolicy.setHeightForWidth(gbSerial->sizePolicy().hasHeightForWidth());
        gbSerial->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(gbSerial);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(gbSerial);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);

        cbSerialPort = new QComboBox(gbSerial);
        cbSerialPort->setObjectName(QString::fromUtf8("cbSerialPort"));
        cbSerialPort->setMinimumSize(QSize(110, 0));
        cbSerialPort->setEditable(true);

        horizontalLayout_4->addWidget(cbSerialPort);


        horizontalLayout_9->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_4 = new QLabel(gbSerial);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_5->addWidget(label_4);

        cbBaudRate = new QComboBox(gbSerial);
        cbBaudRate->addItem(QString());
        cbBaudRate->addItem(QString());
        cbBaudRate->addItem(QString());
        cbBaudRate->addItem(QString());
        cbBaudRate->addItem(QString());
        cbBaudRate->setObjectName(QString::fromUtf8("cbBaudRate"));
        cbBaudRate->setMinimumSize(QSize(90, 0));
        cbBaudRate->setEditable(true);

        horizontalLayout_5->addWidget(cbBaudRate);


        horizontalLayout_9->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_5 = new QLabel(gbSerial);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_6->addWidget(label_5);

        cbDataBits = new QComboBox(gbSerial);
        cbDataBits->addItem(QString());
        cbDataBits->addItem(QString());
        cbDataBits->addItem(QString());
        cbDataBits->addItem(QString());
        cbDataBits->setObjectName(QString::fromUtf8("cbDataBits"));

        horizontalLayout_6->addWidget(cbDataBits);


        horizontalLayout_9->addLayout(horizontalLayout_6);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_6 = new QLabel(gbSerial);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_7->addWidget(label_6);

        cbStopBits = new QComboBox(gbSerial);
        cbStopBits->addItem(QString());
        cbStopBits->addItem(QString());
        cbStopBits->addItem(QString());
        cbStopBits->setObjectName(QString::fromUtf8("cbStopBits"));

        horizontalLayout_7->addWidget(cbStopBits);


        horizontalLayout_10->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_7 = new QLabel(gbSerial);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_8->addWidget(label_7);

        cbParity = new QComboBox(gbSerial);
        cbParity->addItem(QString());
        cbParity->addItem(QString());
        cbParity->addItem(QString());
        cbParity->setObjectName(QString::fromUtf8("cbParity"));

        horizontalLayout_8->addWidget(cbParity);


        horizontalLayout_10->addLayout(horizontalLayout_8);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_2);

        btnOpenPort = new QPushButton(gbSerial);
        btnOpenPort->setObjectName(QString::fromUtf8("btnOpenPort"));
        btnOpenPort->setMinimumSize(QSize(100, 0));
        btnOpenPort->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(0, 200, 0);\n"
"	color: white;\n"
"}\n"
"QPushButton:disabled{\n"
"	background-color:rgb(200, 200, 200);\n"
"	color: rgb(120, 120, 120);\n"
"}"));

        horizontalLayout_10->addWidget(btnOpenPort);

        btnRefreshPorts = new QPushButton(gbSerial);
        btnRefreshPorts->setObjectName(QString::fromUtf8("btnRefreshPorts"));

        horizontalLayout_10->addWidget(btnRefreshPorts);


        verticalLayout_3->addLayout(horizontalLayout_10);


        verticalLayout_2->addWidget(gbSerial);

        gbNetwork = new QGroupBox(tab_2);
        gbNetwork->setObjectName(QString::fromUtf8("gbNetwork"));
        sizePolicy.setHeightForWidth(gbNetwork->sizePolicy().hasHeightForWidth());
        gbNetwork->setSizePolicy(sizePolicy);
        verticalLayout_wifi = new QVBoxLayout(gbNetwork);
        verticalLayout_wifi->setObjectName(QString::fromUtf8("verticalLayout_wifi"));
        horizontalLayout_wifiSettings = new QHBoxLayout();
        horizontalLayout_wifiSettings->setObjectName(QString::fromUtf8("horizontalLayout_wifiSettings"));
        label_8 = new QLabel(gbNetwork);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_wifiSettings->addWidget(label_8);

        leLocalIp = new QLineEdit(gbNetwork);
        leLocalIp->setObjectName(QString::fromUtf8("leLocalIp"));
        leLocalIp->setMinimumSize(QSize(130, 0));
        leLocalIp->setMaximumSize(QSize(180, 16777215));
        leLocalIp->setReadOnly(true);

        horizontalLayout_wifiSettings->addWidget(leLocalIp);

        btnRefreshIp = new QPushButton(gbNetwork);
        btnRefreshIp->setObjectName(QString::fromUtf8("btnRefreshIp"));

        horizontalLayout_wifiSettings->addWidget(btnRefreshIp);

        label_9 = new QLabel(gbNetwork);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_wifiSettings->addWidget(label_9);

        sbListenPort = new QSpinBox(gbNetwork);
        sbListenPort->setObjectName(QString::fromUtf8("sbListenPort"));
        sbListenPort->setMinimumSize(QSize(100, 0));
        sbListenPort->setMaximumSize(QSize(120, 16777215));
        sbListenPort->setMinimum(1);
        sbListenPort->setMaximum(65535);
        sbListenPort->setValue(8080);

        horizontalLayout_wifiSettings->addWidget(sbListenPort);

        btnWifiListen = new QPushButton(gbNetwork);
        btnWifiListen->setObjectName(QString::fromUtf8("btnWifiListen"));
        btnWifiListen->setMinimumSize(QSize(100, 0));
        btnWifiListen->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(0, 200, 0);\n"
"	color: white;\n"
"}\n"
"QPushButton:disabled{\n"
"	background-color:rgb(200, 200, 200);\n"
"	color: rgb(120, 120, 120);\n"
"}"));

        horizontalLayout_wifiSettings->addWidget(btnWifiListen);

        horizontalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_wifiSettings->addItem(horizontalSpacer_3);


        verticalLayout_wifi->addLayout(horizontalLayout_wifiSettings);

        horizontalLayout_wifiStatus = new QHBoxLayout();
        horizontalLayout_wifiStatus->setObjectName(QString::fromUtf8("horizontalLayout_wifiStatus"));
        label_10 = new QLabel(gbNetwork);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_wifiStatus->addWidget(label_10);

        lblWifiStatus = new QLabel(gbNetwork);
        lblWifiStatus->setObjectName(QString::fromUtf8("lblWifiStatus"));
        lblWifiStatus->setMinimumSize(QSize(150, 0));
        lblWifiStatus->setStyleSheet(QString::fromUtf8("color: rgb(120, 120, 120);"));

        horizontalLayout_wifiStatus->addWidget(lblWifiStatus);

        label_15 = new QLabel(gbNetwork);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        horizontalLayout_wifiStatus->addWidget(label_15);

        lblWifiClient = new QLabel(gbNetwork);
        lblWifiClient->setObjectName(QString::fromUtf8("lblWifiClient"));
        lblWifiClient->setMinimumSize(QSize(150, 0));

        horizontalLayout_wifiStatus->addWidget(lblWifiClient);

        label_16 = new QLabel(gbNetwork);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        horizontalLayout_wifiStatus->addWidget(label_16);

        lblWifiStats = new QLabel(gbNetwork);
        lblWifiStats->setObjectName(QString::fromUtf8("lblWifiStats"));

        horizontalLayout_wifiStatus->addWidget(lblWifiStats);

        horizontalSpacer_8 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_wifiStatus->addItem(horizontalSpacer_8);


        verticalLayout_wifi->addLayout(horizontalLayout_wifiStatus);


        verticalLayout_2->addWidget(gbNetwork);

        gbFirmware = new QGroupBox(tab_2);
        gbFirmware->setObjectName(QString::fromUtf8("gbFirmware"));
        sizePolicy.setHeightForWidth(gbFirmware->sizePolicy().hasHeightForWidth());
        gbFirmware->setSizePolicy(sizePolicy);
        verticalLayout_7 = new QVBoxLayout(gbFirmware);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        label_12 = new QLabel(gbFirmware);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_15->addWidget(label_12);

        leFirmwarePath = new QLineEdit(gbFirmware);
        leFirmwarePath->setObjectName(QString::fromUtf8("leFirmwarePath"));

        horizontalLayout_15->addWidget(leFirmwarePath);

        btnSelectFile = new QPushButton(gbFirmware);
        btnSelectFile->setObjectName(QString::fromUtf8("btnSelectFile"));

        horizontalLayout_15->addWidget(btnSelectFile);


        verticalLayout_7->addLayout(horizontalLayout_15);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        label_13 = new QLabel(gbFirmware);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        horizontalLayout_16->addWidget(label_13);

        leFileSize = new QLineEdit(gbFirmware);
        leFileSize->setObjectName(QString::fromUtf8("leFileSize"));
        leFileSize->setMinimumSize(QSize(150, 0));
        leFileSize->setMaximumSize(QSize(300, 16777215));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        leFileSize->setFont(font);
        leFileSize->setFrame(false);
        leFileSize->setReadOnly(true);

        horizontalLayout_16->addWidget(leFileSize);

        label_14 = new QLabel(gbFirmware);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        horizontalLayout_16->addWidget(label_14);

        leCrc16 = new QLineEdit(gbFirmware);
        leCrc16->setObjectName(QString::fromUtf8("leCrc16"));
        leCrc16->setMinimumSize(QSize(120, 0));
        leCrc16->setMaximumSize(QSize(220, 16777215));
        leCrc16->setFont(font);
        leCrc16->setFrame(false);
        leCrc16->setReadOnly(true);

        horizontalLayout_16->addWidget(leCrc16);

        horizontalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_4);


        verticalLayout_7->addLayout(horizontalLayout_16);

        gbUpgradeProgress = new QGroupBox(gbFirmware);
        gbUpgradeProgress->setObjectName(QString::fromUtf8("gbUpgradeProgress"));
        horizontalLayout_17 = new QHBoxLayout(gbUpgradeProgress);
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        progressBar = new QProgressBar(gbUpgradeProgress);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMinimumSize(QSize(250, 0));
        progressBar->setValue(0);

        horizontalLayout_17->addWidget(progressBar);

        btnStartUpgrade = new QPushButton(gbUpgradeProgress);
        btnStartUpgrade->setObjectName(QString::fromUtf8("btnStartUpgrade"));
        btnStartUpgrade->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(0, 200, 0);\n"
"	color: white;\n"
"}\n"
"QPushButton:disabled {\n"
"	background-color: rgb(200, 200, 200);\n"
"	color: rgb(120, 120, 120);\n"
"}"));

        horizontalLayout_17->addWidget(btnStartUpgrade);

        btnStopUpgrade = new QPushButton(gbUpgradeProgress);
        btnStopUpgrade->setObjectName(QString::fromUtf8("btnStopUpgrade"));
        btnStopUpgrade->setEnabled(false);
        btnStopUpgrade->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(220, 40, 40);\n"
"	color: white;\n"
"}\n"
"QPushButton:disabled {\n"
"	background-color: rgb(200, 200, 200);\n"
"	color: rgb(120, 120, 120);\n"
"}"));

        horizontalLayout_17->addWidget(btnStopUpgrade);


        verticalLayout_7->addWidget(gbUpgradeProgress);


        verticalLayout_2->addWidget(gbFirmware);

        gbDevice = new QGroupBox(tab_2);
        gbDevice->setObjectName(QString::fromUtf8("gbDevice"));
        sizePolicy.setHeightForWidth(gbDevice->sizePolicy().hasHeightForWidth());
        gbDevice->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(gbDevice);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(gbDevice);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        leFwVersion = new QLineEdit(gbDevice);
        leFwVersion->setObjectName(QString::fromUtf8("leFwVersion"));
        leFwVersion->setMinimumSize(QSize(140, 0));
        leFwVersion->setReadOnly(true);

        horizontalLayout_2->addWidget(leFwVersion);

        label_2 = new QLabel(gbDevice);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lePartition = new QLineEdit(gbDevice);
        lePartition->setObjectName(QString::fromUtf8("lePartition"));
        lePartition->setMinimumSize(QSize(140, 0));
        lePartition->setReadOnly(true);

        horizontalLayout_2->addWidget(lePartition);

        horizontalSpacer_6 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_6);

        btnReadVersion = new QPushButton(gbDevice);
        btnReadVersion->setObjectName(QString::fromUtf8("btnReadVersion"));

        horizontalLayout_2->addWidget(btnReadVersion);


        verticalLayout_2->addWidget(gbDevice);

        gbLog = new QGroupBox(tab_2);
        gbLog->setObjectName(QString::fromUtf8("gbLog"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(gbLog->sizePolicy().hasHeightForWidth());
        gbLog->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(gbLog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        textLog = new QTextEdit(gbLog);
        textLog->setObjectName(QString::fromUtf8("textLog"));
        textLog->setMinimumSize(QSize(0, 120));
        textLog->setReadOnly(true);

        verticalLayout->addWidget(textLog);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnClearLog = new QPushButton(gbLog);
        btnClearLog->setObjectName(QString::fromUtf8("btnClearLog"));

        horizontalLayout->addWidget(btnClearLog);

        btnSaveLog = new QPushButton(gbLog);
        btnSaveLog->setObjectName(QString::fromUtf8("btnSaveLog"));

        horizontalLayout->addWidget(btnSaveLog);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addWidget(gbLog);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_4 = new QVBoxLayout(tab_3);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(10, 10, 10, 10);
        gbTransferParams = new QGroupBox(tab_3);
        gbTransferParams->setObjectName(QString::fromUtf8("gbTransferParams"));
        sizePolicy.setHeightForWidth(gbTransferParams->sizePolicy().hasHeightForWidth());
        gbTransferParams->setSizePolicy(sizePolicy);
        formLayout = new QFormLayout(gbTransferParams);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        labelChunkSize = new QLabel(gbTransferParams);
        labelChunkSize->setObjectName(QString::fromUtf8("labelChunkSize"));

        formLayout->setWidget(0, QFormLayout::LabelRole, labelChunkSize);

        sbChunkSize = new QSpinBox(gbTransferParams);
        sbChunkSize->setObjectName(QString::fromUtf8("sbChunkSize"));
        sbChunkSize->setMaximumSize(QSize(220, 16777215));
        sbChunkSize->setMinimum(4);
        sbChunkSize->setMaximum(250);
        sbChunkSize->setSingleStep(4);
        sbChunkSize->setValue(128);

        formLayout->setWidget(0, QFormLayout::FieldRole, sbChunkSize);

        labelResponseTimeout = new QLabel(gbTransferParams);
        labelResponseTimeout->setObjectName(QString::fromUtf8("labelResponseTimeout"));

        formLayout->setWidget(1, QFormLayout::LabelRole, labelResponseTimeout);

        sbResponseTimeout = new QDoubleSpinBox(gbTransferParams);
        sbResponseTimeout->setObjectName(QString::fromUtf8("sbResponseTimeout"));
        sbResponseTimeout->setMaximumSize(QSize(220, 16777215));
        sbResponseTimeout->setDecimals(1);
        sbResponseTimeout->setMinimum(0.200000000000000);
        sbResponseTimeout->setMaximum(60.000000000000000);
        sbResponseTimeout->setSingleStep(0.100000000000000);
        sbResponseTimeout->setValue(2.000000000000000);

        formLayout->setWidget(1, QFormLayout::FieldRole, sbResponseTimeout);

        labelEraseTimeout = new QLabel(gbTransferParams);
        labelEraseTimeout->setObjectName(QString::fromUtf8("labelEraseTimeout"));

        formLayout->setWidget(2, QFormLayout::LabelRole, labelEraseTimeout);

        sbEraseTimeout = new QDoubleSpinBox(gbTransferParams);
        sbEraseTimeout->setObjectName(QString::fromUtf8("sbEraseTimeout"));
        sbEraseTimeout->setMaximumSize(QSize(220, 16777215));
        sbEraseTimeout->setDecimals(1);
        sbEraseTimeout->setMinimum(1.000000000000000);
        sbEraseTimeout->setMaximum(120.000000000000000);
        sbEraseTimeout->setSingleStep(0.500000000000000);
        sbEraseTimeout->setValue(15.000000000000000);

        formLayout->setWidget(2, QFormLayout::FieldRole, sbEraseTimeout);

        labelEndTimeout = new QLabel(gbTransferParams);
        labelEndTimeout->setObjectName(QString::fromUtf8("labelEndTimeout"));

        formLayout->setWidget(3, QFormLayout::LabelRole, labelEndTimeout);

        sbEndTimeout = new QDoubleSpinBox(gbTransferParams);
        sbEndTimeout->setObjectName(QString::fromUtf8("sbEndTimeout"));
        sbEndTimeout->setMaximumSize(QSize(220, 16777215));
        sbEndTimeout->setDecimals(1);
        sbEndTimeout->setMinimum(0.500000000000000);
        sbEndTimeout->setMaximum(60.000000000000000);
        sbEndTimeout->setSingleStep(0.500000000000000);
        sbEndTimeout->setValue(5.000000000000000);

        formLayout->setWidget(3, QFormLayout::FieldRole, sbEndTimeout);

        labelFrameDelay = new QLabel(gbTransferParams);
        labelFrameDelay->setObjectName(QString::fromUtf8("labelFrameDelay"));

        formLayout->setWidget(4, QFormLayout::LabelRole, labelFrameDelay);

        sbFrameDelay = new QSpinBox(gbTransferParams);
        sbFrameDelay->setObjectName(QString::fromUtf8("sbFrameDelay"));
        sbFrameDelay->setMaximumSize(QSize(220, 16777215));
        sbFrameDelay->setMaximum(1000);
        sbFrameDelay->setSingleStep(5);
        sbFrameDelay->setValue(0);

        formLayout->setWidget(4, QFormLayout::FieldRole, sbFrameDelay);


        verticalLayout_4->addWidget(gbTransferParams);

        gbProtocolInfo = new QGroupBox(tab_3);
        gbProtocolInfo->setObjectName(QString::fromUtf8("gbProtocolInfo"));
        sizePolicy.setHeightForWidth(gbProtocolInfo->sizePolicy().hasHeightForWidth());
        gbProtocolInfo->setSizePolicy(sizePolicy);
        verticalLayout_6 = new QVBoxLayout(gbProtocolInfo);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        labelProtocolInfo = new QLabel(gbProtocolInfo);
        labelProtocolInfo->setObjectName(QString::fromUtf8("labelProtocolInfo"));
        labelProtocolInfo->setWordWrap(true);

        verticalLayout_6->addWidget(labelProtocolInfo);


        verticalLayout_4->addWidget(gbProtocolInfo);

        labelTransferNote = new QLabel(tab_3);
        labelTransferNote->setObjectName(QString::fromUtf8("labelTransferNote"));
        labelTransferNote->setStyleSheet(QString::fromUtf8("color: rgb(180, 80, 0);"));
        labelTransferNote->setWordWrap(true);

        verticalLayout_4->addWidget(labelTransferNote);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_3);

        tabWidget->addTab(tab_3, QString());

        centralLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        cbDataBits->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "IAP\345\233\272\344\273\266\345\215\207\347\272\247\345\267\245\345\205\267", nullptr));
        gbConnectstyle->setTitle(QApplication::translate("MainWindow", "\350\277\236\346\216\245\346\226\271\345\274\217", nullptr));
        rbSerial->setText(QApplication::translate("MainWindow", "\344\270\262\345\217\243", nullptr));
        rbEth->setText(QApplication::translate("MainWindow", "\344\273\245\345\244\252\347\275\221", nullptr));
        rbWifi->setText(QApplication::translate("MainWindow", "wifi", nullptr));
        gbSerial->setTitle(QApplication::translate("MainWindow", "\344\270\262\345\217\243\350\256\276\347\275\256", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "\344\270\262\345\217\243\345\217\267", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207", nullptr));
        cbBaudRate->setItemText(0, QApplication::translate("MainWindow", "9600", nullptr));
        cbBaudRate->setItemText(1, QApplication::translate("MainWindow", "19200", nullptr));
        cbBaudRate->setItemText(2, QApplication::translate("MainWindow", "38400", nullptr));
        cbBaudRate->setItemText(3, QApplication::translate("MainWindow", "57600", nullptr));
        cbBaudRate->setItemText(4, QApplication::translate("MainWindow", "115200", nullptr));

        cbBaudRate->setCurrentText(QApplication::translate("MainWindow", "9600", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\344\275\215", nullptr));
        cbDataBits->setItemText(0, QApplication::translate("MainWindow", "5", nullptr));
        cbDataBits->setItemText(1, QApplication::translate("MainWindow", "6", nullptr));
        cbDataBits->setItemText(2, QApplication::translate("MainWindow", "7", nullptr));
        cbDataBits->setItemText(3, QApplication::translate("MainWindow", "8", nullptr));

        label_6->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\344\275\215", nullptr));
        cbStopBits->setItemText(0, QApplication::translate("MainWindow", "1", nullptr));
        cbStopBits->setItemText(1, QApplication::translate("MainWindow", "1.5", nullptr));
        cbStopBits->setItemText(2, QApplication::translate("MainWindow", "2", nullptr));

        label_7->setText(QApplication::translate("MainWindow", "\346\240\241\351\252\214\344\275\215 ", nullptr));
        cbParity->setItemText(0, QApplication::translate("MainWindow", "\346\227\240", nullptr));
        cbParity->setItemText(1, QApplication::translate("MainWindow", "\345\245\207", nullptr));
        cbParity->setItemText(2, QApplication::translate("MainWindow", "\345\201\266", nullptr));

        btnOpenPort->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        btnRefreshPorts->setText(QApplication::translate("MainWindow", "\345\210\267\346\226\260\347\253\257\345\217\243", nullptr));
        gbNetwork->setTitle(QApplication::translate("MainWindow", "WiFi \347\233\221\345\220\254\357\274\210PC \344\275\234\344\270\272 TCP \346\234\215\345\212\241\345\231\250\357\274\211", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "\346\234\254\346\234\272 IP", nullptr));
        leLocalIp->setPlaceholderText(QApplication::translate("MainWindow", "\342\200\224", nullptr));
#ifndef QT_NO_TOOLTIP
        btnRefreshIp->setToolTip(QApplication::translate("MainWindow", "\351\207\215\346\226\260\350\257\273\345\217\226\346\234\254\346\234\272 IPv4 \345\234\260\345\235\200", nullptr));
#endif // QT_NO_TOOLTIP
        btnRefreshIp->setText(QApplication::translate("MainWindow", "\345\210\267\346\226\260", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "\347\233\221\345\220\254\347\253\257\345\217\243", nullptr));
        btnWifiListen->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\347\233\221\345\220\254", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "\347\233\221\345\220\254\347\212\266\346\200\201", nullptr));
        lblWifiStatus->setText(QApplication::translate("MainWindow", "\346\234\252\347\233\221\345\220\254", nullptr));
        label_15->setText(QApplication::translate("MainWindow", "\345\256\242\346\210\267\347\253\257", nullptr));
        lblWifiClient->setText(QApplication::translate("MainWindow", "\346\234\252\350\277\236\346\216\245", nullptr));
        label_16->setText(QApplication::translate("MainWindow", "\346\216\245\346\224\266\347\273\237\350\256\241", nullptr));
        lblWifiStats->setText(QApplication::translate("MainWindow", "0 \346\235\241 / 0 \345\255\227\350\212\202", nullptr));
        gbFirmware->setTitle(QApplication::translate("MainWindow", "\345\233\272\344\273\266\345\215\207\347\272\247", nullptr));
        label_12->setText(QApplication::translate("MainWindow", "\345\233\272\344\273\266\346\226\207\344\273\266", nullptr));
        leFirmwarePath->setPlaceholderText(QApplication::translate("MainWindow", "\350\257\267\351\200\211\346\213\251\346\210\226\350\276\223\345\205\245\345\233\272\344\273\266 .bin \346\226\207\344\273\266\350\267\257\345\276\204", nullptr));
        btnSelectFile->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        label_13->setText(QApplication::translate("MainWindow", "\346\226\207\344\273\266\345\244\247\345\260\217", nullptr));
        leFileSize->setPlaceholderText(QApplication::translate("MainWindow", "\342\200\224", nullptr));
        label_14->setText(QApplication::translate("MainWindow", "CRC16", nullptr));
        leCrc16->setPlaceholderText(QApplication::translate("MainWindow", "\342\200\224", nullptr));
        gbUpgradeProgress->setTitle(QApplication::translate("MainWindow", "\345\215\207\347\272\247\350\277\233\345\272\246", nullptr));
        btnStartUpgrade->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\345\215\207\347\272\247", nullptr));
        btnStopUpgrade->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\345\215\207\347\272\247", nullptr));
        gbDevice->setTitle(QApplication::translate("MainWindow", "\350\256\276\345\244\207\344\277\241\346\201\257", nullptr));
        label->setText(QApplication::translate("MainWindow", "\345\233\272\344\273\266\347\211\210\346\234\254", nullptr));
        leFwVersion->setPlaceholderText(QApplication::translate("MainWindow", "\342\200\224", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "\345\275\223\345\211\215\345\210\206\345\214\272", nullptr));
        lePartition->setPlaceholderText(QApplication::translate("MainWindow", "\342\200\224", nullptr));
        btnReadVersion->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226\347\211\210\346\234\254", nullptr));
        gbLog->setTitle(QApplication::translate("MainWindow", "\346\227\245\345\277\227\350\276\223\345\207\272", nullptr));
        btnClearLog->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272\346\227\245\345\277\227", nullptr));
        btnSaveLog->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\346\227\245\345\277\227", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "\350\277\236\346\216\245\350\256\276\347\275\256", nullptr));
        gbTransferParams->setTitle(QApplication::translate("MainWindow", "\344\274\240\350\276\223\345\217\202\346\225\260", nullptr));
        labelChunkSize->setText(QApplication::translate("MainWindow", "\346\257\217\345\270\247\346\225\260\346\215\256\351\225\277\345\272\246", nullptr));
#ifndef QT_NO_TOOLTIP
        sbChunkSize->setToolTip(QApplication::translate("MainWindow", "\345\277\205\351\241\273\346\230\257 4~250 \344\271\213\351\227\264\344\270\224\344\270\272 4 \347\232\204\345\200\215\346\225\260\357\274\210Flash \346\214\211 32 \344\275\215\345\255\227\347\274\226\347\250\213\357\274\211", nullptr));
#endif // QT_NO_TOOLTIP
        sbChunkSize->setSuffix(QApplication::translate("MainWindow", " \345\255\227\350\212\202", nullptr));
        labelResponseTimeout->setText(QApplication::translate("MainWindow", "\346\231\256\351\200\232\345\272\224\347\255\224\350\266\205\346\227\266", nullptr));
        sbResponseTimeout->setSuffix(QApplication::translate("MainWindow", " \347\247\222", nullptr));
        labelEraseTimeout->setText(QApplication::translate("MainWindow", "\346\223\246\351\231\244\345\272\224\347\255\224\350\266\205\346\227\266", nullptr));
        sbEraseTimeout->setSuffix(QApplication::translate("MainWindow", " \347\247\222", nullptr));
        labelEndTimeout->setText(QApplication::translate("MainWindow", "\347\273\223\346\235\237\346\240\241\351\252\214\350\266\205\346\227\266", nullptr));
        sbEndTimeout->setSuffix(QApplication::translate("MainWindow", " \347\247\222", nullptr));
        labelFrameDelay->setText(QApplication::translate("MainWindow", "\345\270\247\351\227\264\345\273\266\346\227\266", nullptr));
        sbFrameDelay->setSuffix(QApplication::translate("MainWindow", " ms", nullptr));
        gbProtocolInfo->setTitle(QApplication::translate("MainWindow", "\345\215\217\350\256\256\350\257\264\346\230\216", nullptr));
        labelProtocolInfo->setText(QApplication::translate("MainWindow", "\345\270\247\346\240\274\345\274\217\357\274\232AA | \345\234\260\345\235\200 | \345\221\275\344\273\244 | \351\225\277\345\272\246 | \346\225\260\346\215\256 | CRC16\357\274\210\344\275\216\345\255\227\350\212\202\345\234\250\345\211\215\357\274\211\n"
"\346\240\241\351\252\214\357\274\232CRC16/Modbus\357\274\214poly 0xA001\357\274\214init 0xFFFF\357\274\214\350\214\203\345\233\264\344\273\216\345\270\247\345\244\264\345\210\260\346\225\260\346\215\256\346\234\253\345\260\276\n"
"App \345\210\206\345\214\272\357\274\2320x08010000 ~ 0x0804FFFF\357\274\214\345\215\225\346\254\241\345\215\207\347\272\247\344\270\212\351\231\220 65535 \345\255\227\350\212\202\n"
"\346\225\260\346\215\256\346\214\211 4 \345\255\227\350\212\202\345\257\271\351\275\220\357\274\214\346\226\207\344\273\266\346\234\253\345\260\276\350\207\252\345\212\250\350\241\245 0xFF", nullptr));
        labelTransferNote->setText(QApplication::translate("MainWindow", "\346\263\250\346\204\217\357\274\232\345\215\217\350\256\256\346\262\241\346\234\211\345\201\217\347\247\273\345\255\227\346\256\265\357\274\214\344\273\273\344\275\225\344\270\200\345\270\247\350\266\205\346\227\266\346\210\226\345\206\231\345\205\245\345\244\261\350\264\245\345\220\216\351\203\275\344\270\215\350\203\275\351\207\215\345\217\221\357\274\233\350\257\267\345\244\215\344\275\215\350\256\276\345\244\207\345\220\216\351\207\215\346\226\260\345\215\207\347\272\247\343\200\202", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "\351\200\232\350\256\257\350\256\276\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
