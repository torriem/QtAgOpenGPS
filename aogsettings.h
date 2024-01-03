#ifndef AOGSETTINGS_H
#define AOGSETTINGS_H

#include <QSettings>
#include <QVariant>
#include <QList>
#include <QVector3D>
#include <QColor>

Q_DECLARE_METATYPE(QList<int>)
Q_DECLARE_METATYPE(QVector<int>)

class QJsonObject;

class AOGSettings : public QSettings
{
public:
    QVariant value(const QString &key, const QVariant &defaultvalue);

    QJsonObject toJson();
    bool saveJson(QString filename);
    bool loadJson(QString filename);
};

QColor parseColor(QString setcolor);
QVector3D parseColorVector(QString setcolor);
int colorSettingStringToInt(QString colorSettingString);

#endif // AOGSETTINGS_H

