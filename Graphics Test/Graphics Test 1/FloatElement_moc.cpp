/****************************************************************************
** Meta object code from reading C++ file 'FloatElement.h'
**
** Created: Tue Feb 13 15:16:31 2018
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "C:\Users\Risona\Documents\OpenGLRenderer\Graphics Test\Graphics Test 1\FloatElement.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FloatElement.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FloatElement[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   14,   13,   13, 0x0a,
      41,   36,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FloatElement[] = {
    "FloatElement\0\0value\0setData(float&)\0"
    "text\0valueChanged(QString)\0"
};

void FloatElement::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FloatElement *_t = static_cast<FloatElement *>(_o);
        switch (_id) {
        case 0: _t->setData((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 1: _t->valueChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FloatElement::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FloatElement::staticMetaObject = {
    { &PropertyElement::staticMetaObject, qt_meta_stringdata_FloatElement,
      qt_meta_data_FloatElement, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FloatElement::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FloatElement::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FloatElement::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FloatElement))
        return static_cast<void*>(const_cast< FloatElement*>(this));
    return PropertyElement::qt_metacast(_clname);
}

int FloatElement::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PropertyElement::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
