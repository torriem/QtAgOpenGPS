#include "qmlsettings.h"
#include <QDateTime>
#include <QSettings>
#include "aogproperty.h"

QMLSettings qml_settings;

QMLSettings::QMLSettings(QObject *parent) :  QQmlPropertyMap(parent) {
    //connect to our own signal so we can map to QSetting and then perform the change
    connect (this, &QQmlPropertyMap::valueChanged, this, &QMLSettings::onValueChanged);
}

void QMLSettings::addKey(QString qml_key, QString settings_key, QString type_name) {
    //map property names to QSettings paths. property names are what are
    //exposed to QML, settings paths are where the property lives in the
    //ini file.
    qml_to_settings_map[qml_key] = settings_key;
    settings_to_qml_map[settings_key] = qml_key;
    settings_type_map[settings_key] = type_name;
}

void QMLSettings::loadSettings() {
    //load every setting that is in our map
    QString settings_key;
    QVariant settings_value;

    for (QString key: qml_to_settings_map.keys()) {
        settings_key = qml_to_settings_map[key];
        settings_value = settings->value(settings_key, unset);

        if (settings_value == unset) continue;

        //fix QVariant type
        if (settings_type_map[settings_key] != "leavealone") {
            auto t = QMetaType::type(settings_type_map[settings_key].toUtf8());
            if (settings_value.userType() != t && !settings_value.convert(t) && !settings_value.isNull()) {
                qWarning() << "Could not set the type of qml setting " << key;
            }
        } else {
            //special case these two which are lists of doubles and integers respectively
            if (settings_key == "tool/zones" && QString(settings_value.typeName()) == "QStringList") {
                QVariantList l;
                for(QString i: settings_value.toStringList()) {
                    l.append(QVariant(i.toInt()));
                }
                settings_value = l;
            } else if (settings_key == "todo/relayPincofig" && QString(settings_value.typeName()) == "QStringList"){
                QVariantList l;
                for(QString i: settings_value.toStringList()) {
                    l.append(QVariant(i.toInt()));
                }
                settings_value = l;
            }
        }

        insert(key, (QVariant) settings_value);
        //qDebug() << "loading " << key << " to " << settings_value;
    }
}

void QMLSettings::updateSetting(const QString &settings_key) {
    //a QSetting key/value pair was updated, so update QML's view
    QString qml_key = settings_to_qml_map[settings_key];
    QVariant settings_value = settings->value(settings_key,unset);

    if (settings_value == unset) return;

    //fix QVariant to have the right type
    if (settings_type_map[settings_key] != "leavealone") {
        auto t = QMetaType::type(settings_type_map[settings_key].toUtf8());
        if (settings_value.userType() != t && !settings_value.convert(t) && !settings_value.isNull()) {
            qWarning() << "Could not set the type of setting " << settings_key;
        }
    } else {
    //special case these two which are lists of doubles and integers respectively
        if (settings_key == "tool/zones" && QString(settings_value.typeName()) == "QStringList") {
            QVariantList l;
            for(QString i: settings_value.toStringList()) {
                l.append(QVariant(i.toInt()));
            }
            settings_value = l;
        } else if (settings_key == "todo/relayPincofig" && QString(settings_value.typeName()) == "QStringList"){
            QVariantList l;
            for(QString i: settings_value.toStringList()) {
                l.append(QVariant(i.toInt()));
            }
            settings_value = l;
        }
     }

    //qDebug() << "updated " << settings_key << " from " << (*this)[qml_key] << " to " << settings_value;

    insert(qml_key, settings_value);
}

void QMLSettings::onValueChanged(const QString &key, const QVariant &value) {
    //Anytime QML changes a property, be sure to update it back to
    //the QSettings store.  We use AOGSetting's setValue_noqml to
    //prevent AOGSetting from calling our updateSetting which would loop!
    QString settings_key = qml_to_settings_map[key];
    QString type_name = value.typeName();
    //qDebug() << "QML changed " << key << " to type " << type_name;
    if (type_name == "QJSValue") {
        //assume QList<int>
        QVector<int> v = toVector<int>(value);
        settings -> setValue_noqml(settings_key, toVariant(v));
    } else {
        settings -> setValue_noqml(settings_key, value);
    }

}
