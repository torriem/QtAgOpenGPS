#include "aogsettings.h"
#include <QString>
#include <QStringList>
#include <QColor>
//#include "common.h"
#include <QDebug>

AOGSettings settings;

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

//TODO: why are these functions here and not in glutils.cpp?
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

int colorSettingStringToInt(QString colorSettingString)
{
    QStringList parts = colorSettingString.split(',');

    int color;

    if (parts.length() > 3)
        color = parts[3].toInt() << 24; //Alpha
    else
        color = 0xff000000;

    color |= parts[0].toInt() << 16; // red
    color |= parts[1].toInt() << 8; // green
    color |= parts[2].toInt();  //blue

    return color;
}

QVector3D parseColorVector(QString setcolor)
{
    //qDebug() << setcolor;

    QStringList c = setcolor.split(",");
    return QVector3D(c.at(0).toInt(),
                     c.at(1).toInt(),
                     c.at(2).toInt());
}
