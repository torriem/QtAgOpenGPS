#include "aogsettings.h"
#include "aogproperty.h"
#include "propdefs.h"

//AOGSettings settings;

AOGProperty test_property("test/test", false);
//QtAOG-specific setting properties
AOGProperty property_environment_last_name("environment/name",QString(""));

AOGProperty::operator int() { return settings->value(key,default_value).toInt(); }
AOGProperty::operator char() { return (char)(settings->value(key,default_value).toInt()); }
AOGProperty::operator double() { return settings->value(key,default_value).toDouble(); }
AOGProperty::operator QString() { return settings->value(key,default_value).toString(); }
AOGProperty::operator bool() { return settings->value(key,default_value).toBool(); }
AOGProperty::operator float() { return settings->value(key,default_value).toFloat(); }
AOGProperty::operator QColor() { return settings->value(key,default_value).value<QColor>(); }
//special case for QVector<int>.  In AOGSettings, this type is marshalled as
//a QVariantList so that the ini file has human-readable text in it.
AOGProperty::operator QVector<int>() { return settings->value(key,default_list); }

QVariant AOGProperty::value(void) { return settings->value(key,default_value); }

AOGProperty &AOGProperty::operator=(QVariant newvalue) { settings->setValue(key, newvalue); return *this;}
AOGProperty &AOGProperty::operator=(QVector<int> newvalue) { settings->setValue(key, newvalue); return *this;}
