#ifndef AOGPROPERTY_H
#define AOGPROPERTY_H

#include "aogsettings.h"
#include <QVector>

extern AOGSettings *settings;

class AOGProperty
{
protected:
    const QString key;
    const QVariant default_value;
    const QVector<int> default_list;

public:
    AOGProperty(const QString key, const QVariant &defaultvalue): key(key), default_value(defaultvalue) {}
    AOGProperty(const QString key, const QVector<int> &defaultvalue): key(key), default_list(defaultvalue) {}

    operator int() { return settings->value(key,default_value).toInt(); }
    operator char() { return (char)(settings->value(key,default_value).toInt()); }
    operator double() { return settings->value(key,default_value).toDouble(); }
    operator QString() { return settings->value(key,default_value).toString(); }
    operator bool() { return settings->value(key,default_value).toBool(); }
    operator float() { return settings->value(key,default_value).toFloat(); }
    operator QColor() { return settings->value(key,default_value).value<QColor>(); }
    //special case for QVector<int>.  In AOGSettings, this type is marshalled as
    //a QVariantList so that the ini file has human-readable text in it.
    operator QVector<int>() { return settings->value(key,default_list); }

    QVariant value(void) { return settings->value(key,default_value); }

    AOGProperty &operator=(QVariant newvalue) { settings->setValue(key, newvalue); return *this;}
};

extern AOGProperty property_environment_last_name;
#include "properties.h"

#endif // AOGPROPERTY_H
