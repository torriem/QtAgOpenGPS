#include "aogsettings.h"

QVariant AOGSettings::value(const QString &key, const QVariant &defaultvalue)
{
    QVariant val;
    val = QSettings::value(key,QVariant::Invalid);
    if (val == QVariant::Invalid) {
        setValue(key,defaultvalue);
        return defaultvalue;
    }

    return val;
}
