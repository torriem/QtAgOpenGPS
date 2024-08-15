#include "agiosettings.h"
#include "agioproperty.h"

//AgIOSettings settings;

AgIOProperty test_property("test/test", false);
//QtAOG-specific setting properties
AgIOProperty property_environment_last_name("environment/name",QString(""));

AgIOProperty::AgIOProperty(const QString key, const QVariant &defaultvalue): key(key), default_value(defaultvalue)
{
    AgIOProperty::add_default_map(key, default_value);
}

AgIOProperty::AgIOProperty(const QString key, const QVector<int> &defaultvalue): key(key), default_list(defaultvalue)
{
    AgIOProperty::add_default_map(key,toVariant(defaultvalue));
}

void AgIOProperty::add_default_map(const QString &key, const QVariant &default_value) {
    default_map[key] = default_value;
}

void AgIOProperty::init_defaults()
{
    for (QString key: AgIOProperty::default_map.keys()) {
        settings->value(key, default_map[key]);
    }
}

AgIOProperty::operator int() { return settings->value(key,default_value).toInt(); }
AOGProperty::operator char() { return (char)(settings->value(key,default_value).toInt()); }
AgIOProperty::operator double() { return settings->value(key,default_value).toDouble(); }
AOGProperty::operator QString() { return settings->value(key,default_value).toString(); }
AgIOProperty::operator bool() { return settings->value(key,default_value).toBool(); }
AOGProperty::operator float() { return settings->value(key,default_value).toFloat(); }
AgIOProperty::operator QColor() { return settings->value(key,default_value).value<QColor>(); }
//special case for QVector<int>.  In AgIOSettings, this type is marshalled as
//a QVariantList so that the ini file has human-readable text in it.
AgIOProperty::operator QVector<int>() { return settings->value(key,default_list); }

QVariant AgIOProperty::value(void) { return settings->value(key,default_value); }

AgIOProperty &AgIOProperty::operator=(QVariant newvalue) { settings->setValue(key, newvalue); return *this;}
AgIOProperty &AgIOProperty::operator=(QVector<int> newvalue) { settings->setValue(key, newvalue); return *this;}
