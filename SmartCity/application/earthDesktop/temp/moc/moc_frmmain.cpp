/****************************************************************************
** Meta object code from reading C++ file 'frmmain.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../frmmain.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'frmmain.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CSplashScreenDlg_t {
    QByteArrayData data[1];
    char stringdata0[17];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CSplashScreenDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CSplashScreenDlg_t qt_meta_stringdata_CSplashScreenDlg = {
    {
QT_MOC_LITERAL(0, 0, 16) // "CSplashScreenDlg"

    },
    "CSplashScreenDlg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CSplashScreenDlg[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void CSplashScreenDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject CSplashScreenDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CSplashScreenDlg.data,
      qt_meta_data_CSplashScreenDlg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CSplashScreenDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CSplashScreenDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CSplashScreenDlg.stringdata0))
        return static_cast<void*>(const_cast< CSplashScreenDlg*>(this));
    if (!strcmp(_clname, "QThread"))
        return static_cast< QThread*>(const_cast< CSplashScreenDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int CSplashScreenDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_frmMain_t {
    QByteArrayData data[19];
    char stringdata0[329];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_frmMain_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_frmMain_t qt_meta_stringdata_frmMain = {
    {
QT_MOC_LITERAL(0, 0, 7), // "frmMain"
QT_MOC_LITERAL(1, 8, 24), // "on_btnMenu_Close_clicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 22), // "on_btnMenu_Max_clicked"
QT_MOC_LITERAL(4, 57, 22), // "on_btnMenu_Min_clicked"
QT_MOC_LITERAL(5, 80, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(6, 102, 23), // "on_pushButton_2_clicked"
QT_MOC_LITERAL(7, 126, 23), // "on_pushButton_3_clicked"
QT_MOC_LITERAL(8, 150, 19), // "slotShowToolControl"
QT_MOC_LITERAL(9, 170, 19), // "slotSearchPlaceName"
QT_MOC_LITERAL(10, 190, 13), // "replyFinished"
QT_MOC_LITERAL(11, 204, 14), // "QNetworkReply*"
QT_MOC_LITERAL(12, 219, 18), // "placeSearchClicked"
QT_MOC_LITERAL(13, 238, 5), // "index"
QT_MOC_LITERAL(14, 244, 14), // "slotDisMeature"
QT_MOC_LITERAL(15, 259, 15), // "slotAreaMeature"
QT_MOC_LITERAL(16, 275, 18), // "slotAzimuthMeature"
QT_MOC_LITERAL(17, 294, 22), // "slotClearMeatureResult"
QT_MOC_LITERAL(18, 317, 11) // "processPipe"

    },
    "frmMain\0on_btnMenu_Close_clicked\0\0"
    "on_btnMenu_Max_clicked\0on_btnMenu_Min_clicked\0"
    "on_pushButton_clicked\0on_pushButton_2_clicked\0"
    "on_pushButton_3_clicked\0slotShowToolControl\0"
    "slotSearchPlaceName\0replyFinished\0"
    "QNetworkReply*\0placeSearchClicked\0"
    "index\0slotDisMeature\0slotAreaMeature\0"
    "slotAzimuthMeature\0slotClearMeatureResult\0"
    "processPipe"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_frmMain[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    0,   90,    2, 0x08 /* Private */,
       4,    0,   91,    2, 0x08 /* Private */,
       5,    0,   92,    2, 0x08 /* Private */,
       6,    0,   93,    2, 0x08 /* Private */,
       7,    0,   94,    2, 0x08 /* Private */,
       8,    0,   95,    2, 0x08 /* Private */,
       9,    0,   96,    2, 0x08 /* Private */,
      10,    1,   97,    2, 0x08 /* Private */,
      12,    1,  100,    2, 0x08 /* Private */,
      14,    0,  103,    2, 0x08 /* Private */,
      15,    0,  104,    2, 0x08 /* Private */,
      16,    0,  105,    2, 0x08 /* Private */,
      17,    0,  106,    2, 0x08 /* Private */,
      18,    0,  107,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,    2,
    QMetaType::Void, QMetaType::QModelIndex,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void frmMain::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        frmMain *_t = static_cast<frmMain *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_btnMenu_Close_clicked(); break;
        case 1: _t->on_btnMenu_Max_clicked(); break;
        case 2: _t->on_btnMenu_Min_clicked(); break;
        case 3: _t->on_pushButton_clicked(); break;
        case 4: _t->on_pushButton_2_clicked(); break;
        case 5: _t->on_pushButton_3_clicked(); break;
        case 6: _t->slotShowToolControl(); break;
        case 7: _t->slotSearchPlaceName(); break;
        case 8: _t->replyFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 9: _t->placeSearchClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 10: _t->slotDisMeature(); break;
        case 11: _t->slotAreaMeature(); break;
        case 12: _t->slotAzimuthMeature(); break;
        case 13: _t->slotClearMeatureResult(); break;
        case 14: _t->processPipe(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        }
    }
}

const QMetaObject frmMain::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_frmMain.data,
      qt_meta_data_frmMain,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *frmMain::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *frmMain::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_frmMain.stringdata0))
        return static_cast<void*>(const_cast< frmMain*>(this));
    return QDialog::qt_metacast(_clname);
}

int frmMain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
