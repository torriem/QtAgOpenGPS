#include "qmlsettings.h"
#include <QDateTime>
#include <QSettings>
#include "aogproperty.h"

QMLSettings qml_settings;

QMLSettings::QMLSettings(QObject *parent) :  QQmlPropertyMap(parent) {
    //connect to our own signal so we can map to QSetting and then perform the change
    connect (this, &QQmlPropertyMap::valueChanged, this, &QMLSettings::onValueChanged);
}

void QMLSettings::addKey(QString qml_key, QString settings_key) {
    //map property names to QSettings paths. property names are what are
    //exposed to QML, settings paths are where the property lives in the
    //ini file.
    qml_to_settings_map[qml_key] = settings_key;
    settings_to_qml_map[settings_key] = qml_key;
}

void QMLSettings::loadSettings() {
    //load every setting that is in our map
    QString settings_key;

    foreach (QString key, qml_to_settings_map.keys()) {
        settings_key = qml_to_settings_map[key];
        insert(key, (QVariant) settings->value(settings_key, QVariant::Invalid));
    }
}

void QMLSettings::updateSetting(const QString &settings_key) {
    //a QSetting key/value pair was updated, so update QML's view
    QString qml_key = settings_to_qml_map[settings_key];
    (*this)[qml_key] = settings->value(settings_key, QVariant::Invalid);
}

void QMLSettings::onValueChanged(const QString &key, const QVariant &value) {
    //Anytime QML changes a property, be sure to update it back to
    //the QSettings store.  We use AOGSetting's setValue_noqml to
    //prevent AOGSetting from calling our updateSetting which would loop!
    QString settings_key = qml_to_settings_map[key];

    settings -> setValue_noqml(settings_key, value);
}
