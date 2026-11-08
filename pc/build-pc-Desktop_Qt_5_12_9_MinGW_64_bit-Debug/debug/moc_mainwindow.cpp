/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../pc/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[60];
    char stringdata0[796];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 17), // "openPortRequested"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 8), // "portName"
QT_MOC_LITERAL(4, 39, 8), // "baudRate"
QT_MOC_LITERAL(5, 48, 8), // "dataBits"
QT_MOC_LITERAL(6, 57, 8), // "stopBits"
QT_MOC_LITERAL(7, 66, 6), // "parity"
QT_MOC_LITERAL(8, 73, 18), // "closePortRequested"
QT_MOC_LITERAL(9, 92, 20), // "readVersionRequested"
QT_MOC_LITERAL(10, 113, 9), // "timeoutMs"
QT_MOC_LITERAL(11, 123, 21), // "startUpgradeRequested"
QT_MOC_LITERAL(12, 145, 5), // "image"
QT_MOC_LITERAL(13, 151, 9), // "chunkSize"
QT_MOC_LITERAL(14, 161, 17), // "responseTimeoutMs"
QT_MOC_LITERAL(15, 179, 14), // "eraseTimeoutMs"
QT_MOC_LITERAL(16, 194, 12), // "endTimeoutMs"
QT_MOC_LITERAL(17, 207, 12), // "frameDelayMs"
QT_MOC_LITERAL(18, 220, 18), // "refreshSerialPorts"
QT_MOC_LITERAL(19, 239, 10), // "togglePort"
QT_MOC_LITERAL(20, 250, 23), // "onConnectionModeToggled"
QT_MOC_LITERAL(21, 274, 7), // "checked"
QT_MOC_LITERAL(22, 282, 21), // "refreshLocalAddresses"
QT_MOC_LITERAL(23, 304, 19), // "toggleWifiListening"
QT_MOC_LITERAL(24, 324, 22), // "onWifiListeningStarted"
QT_MOC_LITERAL(25, 347, 4), // "port"
QT_MOC_LITERAL(26, 352, 21), // "onWifiListeningFailed"
QT_MOC_LITERAL(27, 374, 5), // "error"
QT_MOC_LITERAL(28, 380, 22), // "onWifiListeningStopped"
QT_MOC_LITERAL(29, 403, 21), // "onWifiClientConnected"
QT_MOC_LITERAL(30, 425, 4), // "peer"
QT_MOC_LITERAL(31, 430, 24), // "onWifiClientDisconnected"
QT_MOC_LITERAL(32, 455, 13), // "onWifiMessage"
QT_MOC_LITERAL(33, 469, 21), // "WifiListener::Message"
QT_MOC_LITERAL(34, 491, 7), // "message"
QT_MOC_LITERAL(35, 499, 12), // "onPortOpened"
QT_MOC_LITERAL(36, 512, 11), // "description"
QT_MOC_LITERAL(37, 524, 16), // "onPortOpenFailed"
QT_MOC_LITERAL(38, 541, 12), // "onPortClosed"
QT_MOC_LITERAL(39, 554, 15), // "onVersionResult"
QT_MOC_LITERAL(40, 570, 2), // "ok"
QT_MOC_LITERAL(41, 573, 7), // "payload"
QT_MOC_LITERAL(42, 581, 17), // "onProgressChanged"
QT_MOC_LITERAL(43, 599, 4), // "sent"
QT_MOC_LITERAL(44, 604, 5), // "total"
QT_MOC_LITERAL(45, 610, 4), // "kbps"
QT_MOC_LITERAL(46, 615, 17), // "onUpgradeFinished"
QT_MOC_LITERAL(47, 633, 9), // "cancelled"
QT_MOC_LITERAL(48, 643, 7), // "summary"
QT_MOC_LITERAL(49, 651, 18), // "onPartitionChanged"
QT_MOC_LITERAL(50, 670, 9), // "partition"
QT_MOC_LITERAL(51, 680, 18), // "selectFirmwareFile"
QT_MOC_LITERAL(52, 699, 20), // "onFirmwarePathEdited"
QT_MOC_LITERAL(53, 720, 12), // "startUpgrade"
QT_MOC_LITERAL(54, 733, 11), // "stopUpgrade"
QT_MOC_LITERAL(55, 745, 17), // "readDeviceVersion"
QT_MOC_LITERAL(56, 763, 7), // "saveLog"
QT_MOC_LITERAL(57, 771, 8), // "clearLog"
QT_MOC_LITERAL(58, 780, 9), // "appendLog"
QT_MOC_LITERAL(59, 790, 5) // "level"

    },
    "MainWindow\0openPortRequested\0\0portName\0"
    "baudRate\0dataBits\0stopBits\0parity\0"
    "closePortRequested\0readVersionRequested\0"
    "timeoutMs\0startUpgradeRequested\0image\0"
    "chunkSize\0responseTimeoutMs\0eraseTimeoutMs\0"
    "endTimeoutMs\0frameDelayMs\0refreshSerialPorts\0"
    "togglePort\0onConnectionModeToggled\0"
    "checked\0refreshLocalAddresses\0"
    "toggleWifiListening\0onWifiListeningStarted\0"
    "port\0onWifiListeningFailed\0error\0"
    "onWifiListeningStopped\0onWifiClientConnected\0"
    "peer\0onWifiClientDisconnected\0"
    "onWifiMessage\0WifiListener::Message\0"
    "message\0onPortOpened\0description\0"
    "onPortOpenFailed\0onPortClosed\0"
    "onVersionResult\0ok\0payload\0onProgressChanged\0"
    "sent\0total\0kbps\0onUpgradeFinished\0"
    "cancelled\0summary\0onPartitionChanged\0"
    "partition\0selectFirmwareFile\0"
    "onFirmwarePathEdited\0startUpgrade\0"
    "stopUpgrade\0readDeviceVersion\0saveLog\0"
    "clearLog\0appendLog\0level"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      30,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    5,  164,    2, 0x06 /* Public */,
       8,    0,  175,    2, 0x06 /* Public */,
       9,    1,  176,    2, 0x06 /* Public */,
      11,    6,  179,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      18,    0,  192,    2, 0x08 /* Private */,
      19,    0,  193,    2, 0x08 /* Private */,
      20,    1,  194,    2, 0x08 /* Private */,
      22,    0,  197,    2, 0x08 /* Private */,
      23,    0,  198,    2, 0x08 /* Private */,
      24,    1,  199,    2, 0x08 /* Private */,
      26,    1,  202,    2, 0x08 /* Private */,
      28,    0,  205,    2, 0x08 /* Private */,
      29,    1,  206,    2, 0x08 /* Private */,
      31,    1,  209,    2, 0x08 /* Private */,
      32,    1,  212,    2, 0x08 /* Private */,
      35,    1,  215,    2, 0x08 /* Private */,
      37,    1,  218,    2, 0x08 /* Private */,
      38,    0,  221,    2, 0x08 /* Private */,
      39,    3,  222,    2, 0x08 /* Private */,
      42,    3,  229,    2, 0x08 /* Private */,
      46,    3,  236,    2, 0x08 /* Private */,
      49,    1,  243,    2, 0x08 /* Private */,
      51,    0,  246,    2, 0x08 /* Private */,
      52,    0,  247,    2, 0x08 /* Private */,
      53,    0,  248,    2, 0x08 /* Private */,
      54,    0,  249,    2, 0x08 /* Private */,
      55,    0,  250,    2, 0x08 /* Private */,
      56,    0,  251,    2, 0x08 /* Private */,
      57,    0,  252,    2, 0x08 /* Private */,
      58,    2,  253,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,    6,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   12,   13,   14,   15,   16,   17,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   21,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UShort,   25,
    QMetaType::Void, QMetaType::QString,   27,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   30,
    QMetaType::Void, QMetaType::QString,   30,
    QMetaType::Void, 0x80000000 | 33,   34,
    QMetaType::Void, QMetaType::QString,   36,
    QMetaType::Void, QMetaType::QString,   27,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::QByteArray, QMetaType::QString,   40,   41,   34,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Double,   43,   44,   45,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool, QMetaType::QString,   40,   47,   48,
    QMetaType::Void, QMetaType::QString,   50,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   34,   59,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->openPortRequested((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 1: _t->closePortRequested(); break;
        case 2: _t->readVersionRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->startUpgradeRequested((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 4: _t->refreshSerialPorts(); break;
        case 5: _t->togglePort(); break;
        case 6: _t->onConnectionModeToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->refreshLocalAddresses(); break;
        case 8: _t->toggleWifiListening(); break;
        case 9: _t->onWifiListeningStarted((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 10: _t->onWifiListeningFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->onWifiListeningStopped(); break;
        case 12: _t->onWifiClientConnected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->onWifiClientDisconnected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->onWifiMessage((*reinterpret_cast< const WifiListener::Message(*)>(_a[1]))); break;
        case 15: _t->onPortOpened((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: _t->onPortOpenFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->onPortClosed(); break;
        case 18: _t->onVersionResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 19: _t->onProgressChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 20: _t->onUpgradeFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 21: _t->onPartitionChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 22: _t->selectFirmwareFile(); break;
        case 23: _t->onFirmwarePathEdited(); break;
        case 24: _t->startUpgrade(); break;
        case 25: _t->stopUpgrade(); break;
        case 26: _t->readDeviceVersion(); break;
        case 27: _t->saveLog(); break;
        case 28: _t->clearLog(); break;
        case 29: _t->appendLog((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 14:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< WifiListener::Message >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(const QString & , int , int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::openPortRequested)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::closePortRequested)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::readVersionRequested)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(const QByteArray & , int , int , int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::startUpgradeRequested)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 30)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 30;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 30)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 30;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::openPortRequested(const QString & _t1, int _t2, int _t3, int _t4, int _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::closePortRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MainWindow::readVersionRequested(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindow::startUpgradeRequested(const QByteArray & _t1, int _t2, int _t3, int _t4, int _t5, int _t6)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
