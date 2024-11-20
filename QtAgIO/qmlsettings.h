#ifndef QMLSETTINGS_H
#define QMLSETTINGS_H

#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqml.h>
#include <QVariantList>
#include <QSettings>
#include <QQmlPropertyMap>


class QMLSettings : public QQmlPropertyMap
{
    Q_OBJECT
public:
    QMap<QString, QString> qml_to_settings_map; //map of qml property keys to settings path keys
    QMap<QString, QString> settings_to_qml_map; //map of qml property keys to settings path keys
    QMap<QString, QString> settings_type_map; //map of qml property keys to settings path keys

    QMLSettings(QObject *parent = 0);

    void addKey(QString qml_key, QString settings_key, QString type_name);
    void setupKeys();

    void loadSettings();

    void updateSetting(const QString &settings_key);
public slots:
    void onValueChanged(const QString &key, const QVariant &value);
};

#endif // QMLSETTINGS_H
