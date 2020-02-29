#include "aogsettings.h"
#include <QString>
#include <QStringList>
#include <QColor>
#include "common.h"
#include <QDebug>

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

QColor parseColor(QString setcolor)
{
    //qDebug() << setcolor;

    QStringList c = setcolor.split(",");
    if (c.size() == 3) {
        return QColor::fromRgb(c.at(0).toInt(),
                               c.at(1).toInt(),
                               c.at(2).toInt());
    } else if (c.size() == 4) {
        return QColor::fromRgb(c.at(0).toInt(),
                               c.at(1).toInt(),
                               c.at(2).toInt(),
                               c.at(3).toInt());
    } else {
       return QColor::fromRgb(255,0,255); //return magenta so we can see it
    }
}
