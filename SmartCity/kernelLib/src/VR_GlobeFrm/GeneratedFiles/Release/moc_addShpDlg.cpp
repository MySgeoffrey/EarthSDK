/****************************************************************************
** Meta object code from reading C++ file 'addShpDlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../addShpDlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'addShpDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Globe__CAddShp_t {
    QByteArrayData data[8];
    char stringdata0[109];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Globe__CAddShp_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Globe__CAddShp_t qt_meta_stringdata_Globe__CAddShp = {
    {
QT_MOC_LITERAL(0, 0, 14), // "Globe::CAddShp"
QT_MOC_LITERAL(1, 15, 17), // "createVectorLayer"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 21), // "osgEarth::ImageLayer*"
QT_MOC_LITERAL(4, 56, 12), // "pVectorLayer"
QT_MOC_LITERAL(5, 69, 15), // "slotOpenFileDlg"
QT_MOC_LITERAL(6, 85, 9), // "slotOkBtn"
QT_MOC_LITERAL(7, 95, 13) // "slotCancleBtn"

    },
    "Globe::CAddShp\0createVectorLayer\0\0"
    "osgEarth::ImageLayer*\0pVectorLayer\0"
    "slotOpenFileDlg\0slotOkBtn\0slotCancleBtn"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Globe__CAddShp[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   37,    2, 0x08 /* Private */,
       6,    0,   38,    2, 0x08 /* Private */,
       7,    0,   39,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Globe::CAddShp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CAddShp *_t = static_cast<CAddShp *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->createVectorLayer((*reinterpret_cast< osgEarth::ImageLayer*(*)>(_a[1]))); break;
        case 1: _t->slotOpenFileDlg(); break;
        case 2: _t->slotOkBtn(); break;
        case 3: _t->slotCancleBtn(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CAddShp::*_t)(osgEarth::ImageLayer * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CAddShp::createVectorLayer)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Globe::CAddShp::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Globe__CAddShp.data,
      qt_meta_data_Globe__CAddShp,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Globe::CAddShp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Globe::CAddShp::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Globe__CAddShp.stringdata0))
        return static_cast<void*>(const_cast< CAddShp*>(this));
    return QDialog::qt_metacast(_clname);
}

int Globe::CAddShp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Globe::CAddShp::createVectorLayer(osgEarth::ImageLayer * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
