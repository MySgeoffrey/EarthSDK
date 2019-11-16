/****************************************************************************
** Meta object code from reading C++ file 'addDomDlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../addDomDlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'addDomDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Globe__CAddDom_t {
    QByteArrayData data[9];
    char stringdata0[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Globe__CAddDom_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Globe__CAddDom_t qt_meta_stringdata_Globe__CAddDom = {
    {
QT_MOC_LITERAL(0, 0, 14), // "Globe::CAddDom"
QT_MOC_LITERAL(1, 15, 19), // "signalAddLocalImage"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 21), // "osgEarth::ImageLayer*"
QT_MOC_LITERAL(4, 58, 11), // "pImageLayer"
QT_MOC_LITERAL(5, 70, 19), // "slotOpenFileGdalDlg"
QT_MOC_LITERAL(6, 90, 18), // "slotOpenFileTmsDlg"
QT_MOC_LITERAL(7, 109, 9), // "slotOkBtn"
QT_MOC_LITERAL(8, 119, 13) // "slotCancleBtn"

    },
    "Globe::CAddDom\0signalAddLocalImage\0\0"
    "osgEarth::ImageLayer*\0pImageLayer\0"
    "slotOpenFileGdalDlg\0slotOpenFileTmsDlg\0"
    "slotOkBtn\0slotCancleBtn"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Globe__CAddDom[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   42,    2, 0x08 /* Private */,
       6,    0,   43,    2, 0x08 /* Private */,
       7,    0,   44,    2, 0x08 /* Private */,
       8,    0,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Globe::CAddDom::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CAddDom *_t = static_cast<CAddDom *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalAddLocalImage((*reinterpret_cast< osgEarth::ImageLayer*(*)>(_a[1]))); break;
        case 1: _t->slotOpenFileGdalDlg(); break;
        case 2: _t->slotOpenFileTmsDlg(); break;
        case 3: _t->slotOkBtn(); break;
        case 4: _t->slotCancleBtn(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CAddDom::*_t)(osgEarth::ImageLayer * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CAddDom::signalAddLocalImage)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Globe::CAddDom::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Globe__CAddDom.data,
      qt_meta_data_Globe__CAddDom,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Globe::CAddDom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Globe::CAddDom::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Globe__CAddDom.stringdata0))
        return static_cast<void*>(const_cast< CAddDom*>(this));
    return QDialog::qt_metacast(_clname);
}

int Globe::CAddDom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Globe::CAddDom::signalAddLocalImage(osgEarth::ImageLayer * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
