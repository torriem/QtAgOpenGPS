// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// I'm going to attempt my documentation in the backend. I'm rather lost there, still. If something is wrong, please fix it.
// I will only document in the .h files, unless several .cpp's are using one .h (like formgps.h)
//
// Settings wrapper class tie-in  
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
    inline static QMap<QString, QVariant> default_map;
    static void add_default_map (const QString &key, const QVariant &default_value);
    static void init_defaults();

    AOGProperty(const QString key, const QVariant &defaultvalue);
    AOGProperty(const QString key, const QVector<int> &defaultvalue);

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
