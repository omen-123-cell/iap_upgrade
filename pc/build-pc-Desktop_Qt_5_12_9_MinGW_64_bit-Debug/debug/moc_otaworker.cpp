/****************************************************************************
** Meta object code from reading C++ file 'otaworker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../pc/otaworker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'otaworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OtaWorker_t {
    QByteArrayData data[38];
    char stringdata0[381];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OtaWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OtaWorker_t qt_meta_stringdata_OtaWorker = {
    {
QT_MOC_LITERAL(0, 0, 9), // "OtaWorker"
QT_MOC_LITERAL(1, 10, 10), // "logMessage"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 7), // "message"
QT_MOC_LITERAL(4, 30, 5), // "level"
QT_MOC_LITERAL(5, 36, 10), // "portOpened"
QT_MOC_LITERAL(6, 47, 11), // "description"
QT_MOC_LITERAL(7, 59, 14), // "portOpenFailed"
QT_MOC_LITERAL(8, 74, 5), // "error"
QT_MOC_LITERAL(9, 80, 10), // "portClosed"
QT_MOC_LITERAL(10, 91, 13), // "versionResult"
QT_MOC_LITERAL(11, 105, 2), // "ok"
QT_MOC_LITERAL(12, 108, 7), // "payload"
QT_MOC_LITERAL(13, 116, 15), // "progressChanged"
QT_MOC_LITERAL(14, 132, 4), // "sent"
QT_MOC_LITERAL(15, 137, 5), // "total"
QT_MOC_LITERAL(16, 143, 4), // "kbps"
QT_MOC_LITERAL(17, 148, 15), // "upgradeFinished"
QT_MOC_LITERAL(18, 164, 9), // "cancelled"
QT_MOC_LITERAL(19, 174, 7), // "summary"
QT_MOC_LITERAL(20, 182, 16), // "partitionChanged"
QT_MOC_LITERAL(21, 199, 9), // "partition"
QT_MOC_LITERAL(22, 209, 8), // "openPort"
QT_MOC_LITERAL(23, 218, 8), // "portName"
QT_MOC_LITERAL(24, 227, 8), // "baudRate"
QT_MOC_LITERAL(25, 236, 8), // "dataBits"
QT_MOC_LITERAL(26, 245, 8), // "stopBits"
QT_MOC_LITERAL(27, 254, 6), // "parity"
QT_MOC_LITERAL(28, 261, 9), // "closePort"
QT_MOC_LITERAL(29, 271, 11), // "readVersion"
QT_MOC_LITERAL(30, 283, 9), // "timeoutMs"
QT_MOC_LITERAL(31, 293, 12), // "startUpgrade"
QT_MOC_LITERAL(32, 306, 5), // "image"
QT_MOC_LITERAL(33, 312, 9), // "chunkSize"
QT_MOC_LITERAL(34, 322, 17), // "responseTimeoutMs"
QT_MOC_LITERAL(35, 340, 14), // "eraseTimeoutMs"
QT_MOC_LITERAL(36, 355, 12), // "endTimeoutMs"
QT_MOC_LITERAL(37, 368, 12) // "frameDelayMs"

    },
    "OtaWorker\0logMessage\0\0message\0level\0"
    "portOpened\0description\0portOpenFailed\0"
    "error\0portClosed\0versionResult\0ok\0"
    "payload\0progressChanged\0sent\0total\0"
    "kbps\0upgradeFinished\0cancelled\0summary\0"
    "partitionChanged\0partition\0openPort\0"
    "portName\0baudRate\0dataBits\0stopBits\0"
    "parity\0closePort\0readVersion\0timeoutMs\0"
    "startUpgrade\0image\0chunkSize\0"
    "responseTimeoutMs\0eraseTimeoutMs\0"
    "endTimeoutMs\0frameDelayMs"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OtaWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   74,    2, 0x06 /* Public */,
       5,    1,   79,    2, 0x06 /* Public */,
       7,    1,   82,    2, 0x06 /* Public */,
       9,    0,   85,    2, 0x06 /* Public */,
      10,    3,   86,    2, 0x06 /* Public */,
      13,    3,   93,    2, 0x06 /* Public */,
      17,    3,  100,    2, 0x06 /* Public */,
      20,    1,  107,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      22,    5,  110,    2, 0x0a /* Public */,
      28,    0,  121,    2, 0x0a /* Public */,
      29,    1,  122,    2, 0x0a /* Public */,
      31,    6,  125,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::QByteArray, QMetaType::QString,   11,   12,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Double,   14,   15,   16,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool, QMetaType::QString,   11,   18,   19,
    QMetaType::Void, QMetaType::QString,   21,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   23,   24,   25,   26,   27,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   30,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   32,   33,   34,   35,   36,   37,

       0        // eod
};

void OtaWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OtaWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->logMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->portOpened((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->portOpenFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->portClosed(); break;
        case 4: _t->versionResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 5: _t->progressChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 6: _t->upgradeFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 7: _t->partitionChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->openPort((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 9: _t->closePort(); break;
        case 10: _t->readVersion((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->startUpgrade((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OtaWorker::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OtaWorker::logMessage)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OtaWorker::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OtaWorker::portOpened)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (OtaWorker::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OtaWorker::portOpenFailed)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (OtaWorker::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OtaWorker::portClosed)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (OtaWorker::*)(bool , const QByteArray & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OtaWorker::versionResult)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (OtaWorker::*)(int , int , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OtaWorker::progressChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (OtaWorker::*)(bool , bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OtaWorker::upgradeFinished)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (OtaWorker::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OtaWorker::partitionChanged)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject OtaWorker::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_OtaWorker.data,
    qt_meta_data_OtaWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *OtaWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OtaWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OtaWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int OtaWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void OtaWorker::logMessage(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OtaWorker::portOpened(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void OtaWorker::portOpenFailed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void OtaWorker::portClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void OtaWorker::versionResult(bool _t1, const QByteArray & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void OtaWorker::progressChanged(int _t1, int _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void OtaWorker::upgradeFinished(bool _t1, bool _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void OtaWorker::partitionChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
