#include "aogsettings.h"
#include <QString>
#include <QStringList>
#include <QColor>
//#include "common.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDataStream>
#include <QFile>

QVariant AOGSettings::value(const QString &key, const QVariant &defaultvalue)
{
    QVariant val;
    val = QSettings::value(key,QVariant::Invalid);
    if (val == QVariant::Invalid) {
        QSettings::setValue(key,defaultvalue);
        return defaultvalue;
    }

    return val;
}

QVector<int> AOGSettings::value(const QString &key, const QVector<int> &defaultvalue)
{
    QVariant val;
    val = QSettings::value(key,QVariant::Invalid);
    if (val == QVariant::Invalid) {
        QSettings::setValue(key,toVariant(defaultvalue));
        return defaultvalue;
    }

    return toVector<int>(val);
}


void AOGSettings::setValue(const QString &key, const QVector<int> &value_list)
{
    QSettings::setValue(key,toVariant(value_list));
}

void AOGSettings::setValue(const QString &key, const QVariant &value)
{
    QSettings::setValue(key,value);
}

QJsonObject AOGSettings::toJson()
{
    //b = QVariant(QColor::fromRgbF(1,0.5,0.2));
    QVariant b;

    QStringList keys = allKeys();
    QString type;
    QString json_value;
    QJsonObject blah; // = QJsonObject::fromVariantMap(keysValuesPairs);

    for (const auto &key : keys)
    {
        b = value(key,QVariant::Invalid);

        type = b.typeName();

        /*
        if (type == "double" ||
            type == "int" ||
            type == "char" ||
            type == "QString" ||
            type == "bool" ||
            type == "float"
            )
        {
        */
            json_value = b.toString();
        qWarning() << key <<", " << type << ", " << json_value;
        /*
        } else {
            QByteArray raw_value;
            QDataStream ds(&raw_value,QIODevice::WriteOnly);

            ds << b;

            json_value = QLatin1String("@Variant(");
            json_value += QString::fromLatin1(raw_value.constData(), raw_value.size());
            json_value += ")";
        }
        */

        blah[key] = json_value;
    }


    /*
    QMap<QString, QVariant> keysValuesPairs;
    keysValuesPairs.insert("testing",QVariant(QColor::fromRgbF(1,0.5,0.25)));

    */
    return blah;

}

bool AOGSettings::loadJson(QString filename)
{
    QFile loadfile(filename);
    if (!loadfile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Could not load json settings file " << filename;
        return false;
    }

    QByteArray loadedjson = loadfile.readAll();
    QJsonDocument loaded(QJsonDocument::fromJson(loadedjson));
    QJsonObject j = loaded.object();
    QString new_value;
    QVariant v;

    for (const auto &key : j.keys())
    {
        new_value = j[key].toString();
        if (new_value.startsWith("@Variant("))
        {
            QByteArray raw_data;
            QDataStream ds(&raw_data,QIODevice::ReadOnly);

            raw_data = new_value.toLatin1().mid(9);
            ds >> v;
        } else
        {
            v = QVariant(new_value);
        }

        QSettings::setValue(key, v);
    }

    return true;

}

bool AOGSettings::saveJson(QString filename)
{
    QFile savefile(filename);
    if (!savefile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Could not save json settings file " << filename;
        return false;
    }

    savefile.write(QJsonDocument(toJson()).toJson());
    savefile.close();

    return true;

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
