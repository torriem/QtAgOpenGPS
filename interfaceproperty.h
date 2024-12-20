#ifndef INTERFACEPROPERTY_H
#define INTERFACEPROPERTY_H

#include <QString>
#include <QVariant>
#include "btnenum.h"

class AOGInterface;
class FieldInterface;
//class VehicleInterface;
//class LinesInterface;
class BoundaryInterface;
class RecordedPathInterface;

template <typename WhichInterface, class T>
class InterfaceProperty {
protected:
    QByteArray prop_name_bytes;
    const char *prop_name;
public:
    static inline QObject *prop_root = NULL;
    static inline void set_qml_root(QObject *aogiface_root) {prop_root = aogiface_root; }

    InterfaceProperty(const char *qml_prop_name)
    {
        prop_name_bytes = QString(qml_prop_name).toUtf8();
        prop_name = prop_name_bytes.constData();
    }

    InterfaceProperty() {}

    ~InterfaceProperty() {
    }


    //TODO add types as needed
    inline operator bool() const { return prop_root->property(prop_name).toBool(); }
    inline operator bool() { return prop_root->property(prop_name).toBool(); }
    //inline operator bool() const { return prop_root->property(prop_name).toBool(); }
    inline operator int() { return prop_root->property(prop_name).toInt(); }
    inline operator uint() { return prop_root->property(prop_name).toUInt(); }
    inline operator double() { return prop_root->property(prop_name).toDouble(); }
    inline operator btnStates() { return static_cast<btnStates>(prop_root->property(prop_name).toInt()); }

    //operators for int and double
    inline InterfaceProperty &operator+=(T rhs) {
        prop_root->setProperty(prop_name, QVariant((T)(*this) + rhs));

        return *this;
    }
    inline InterfaceProperty &operator-=(double rhs) {
        prop_root->setProperty(prop_name, QVariant((T)(*this) - rhs));

        return *this;
    }

    inline InterfaceProperty &operator=(T newvalue) { prop_root->setProperty(prop_name, QVariant(newvalue)); return *this;}

};

#endif // INTERFACEPROPERTY_H
