/****************************************************************************
** Meta object code from reading C++ file 'addModelDlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../addModelDlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'addModelDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Globe__CAddModel_t {
    QByteArrayData data[5];
    char stringdata0[55];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Globe__CAddModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Globe__CAddModel_t qt_meta_stringdata_Globe__CAddModel = {
    {
QT_MOC_LITERAL(0, 0, 16), // "Globe::CAddModel"
QT_MOC_LITERAL(1, 17, 12), // "slotOpenFile"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 9), // "slotOkBtn"
QT_MOC_LITERAL(4, 41, 13) // "slotCancleBtn"

    },
    "Globe::CAddModel\0slotOpenFile\0\0slotOkBtn\0"
    "slotCancleBtn"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Globe__CAddModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    0,   30,    2, 0x08 /* Private */,
       4,    0,   31,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Globe::CAddModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CAddModel *_t = static_cast<CAddModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotOpenFile(); break;
        case 1: _t->slotOkBtn(); break;
        case 2: _t->slotCancleBtn(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Globe::CAddModel::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Globe__CAddModel.data,
      qt_meta_data_Globe__CAddModel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Globe::CAddModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Globe::CAddModel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Globe__CAddModel.stringdata0))
        return static_cast<void*>(const_cast< CAddModel*>(this));
    return QDialog::qt_metacast(_clname);
}

int Globe::CAddModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
