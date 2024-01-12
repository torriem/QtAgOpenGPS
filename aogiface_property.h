#ifndef AOGIFACE_PROPERTY_H
#define AOGIFACE_PROPERTY_H

#include <QString>
#include <QVariant>
#include "btnenum.h"

template <class T>
class AOGIFace_Property {
    char * prop_name;
public:
    static inline QObject *prop_root = NULL;

    AOGIFace_Property(const char *qml_prop_name)
    {
        prop_name = strdup(qml_prop_name);
    }

    AOGIFace_Property() : prop_name(NULL) {}

    //inline operator QString() { return prop_root->property(prop_name).toString(); }
    //TODO add types as needed
    static inline void set_qml_root(QObject *aogiface_root) {prop_root = aogiface_root; }
    inline operator bool() { return prop_root->property(prop_name).toBool(); }
    inline operator int() { return prop_root->property(prop_name).toInt(); }
    inline operator btnStates() { return static_cast<btnStates>(prop_root->property(prop_name).toInt()); }

    inline AOGIFace_Property &operator=(T newvalue) { prop_root->setProperty(prop_name, QVariant(newvalue)); return *this;}

};

#endif // AOGIFACE_PROPERTY_H
