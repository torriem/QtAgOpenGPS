#ifndef AOGPROPERTY_H
#define AOGPROPERTY_H

#include "aogsettings.h"

extern AOGSettings settings;

class AOGProperty
{
protected:
    const QString key;
    const QVariant default_value;

public:
    AOGProperty(const QString key, const QVariant &defaultvalue): key(key), default_value(defaultvalue) {}

    operator int() { return settings.value(key,default_value).toInt(); }
    operator double() { return settings.value(key,default_value).toDouble(); }
    operator QString() { return settings.value(key,default_value).toString(); }
    operator bool() { return settings.value(key,default_value).toBool(); }

    QVariant value(void) { return settings.value(key,default_value); }

    AOGProperty &operator=(QVariant newvalue) { settings.setValue(key, newvalue); return *this;}
};

#endif // AOGPROPERTY_H
