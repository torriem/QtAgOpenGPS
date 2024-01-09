#ifndef AOGPROPERTY_H
#define AOGPROPERTY_H

#include <QVector>
#include <QVariant>

class AOGSettings;

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

    operator int();
    operator char();
    operator double();
    operator QString();
    operator bool();
    operator float();
    operator QColor();
    //special case for QVector<int>.  In AOGSettings, this type is marshalled as
    //a QVariantList so that the ini file has human-readable text in it.
    operator QVector<int>();

    QVariant value(void);

    AOGProperty &operator=(QVariant newvalue);
    AOGProperty &operator=(QVector<int> newvalue);
};

extern AOGProperty property_environment_last_name;
#include "properties.h"

#endif // AOGPROPERTY_H
