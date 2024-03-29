#include "formgps.h"
#include "qmlutil.h"
#include "aogproperty.h"
#include "qmlsettings.h"

QString caseInsensitiveFilename(QString directory, QString filename);

extern QMLSettings qml_settings;

void FormGPS::vehicle_saveas(QString vehicle_name) {
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/" + QCoreApplication::applicationName() + "/Vehicles";

    QDir saveDir(directoryName);
    if (!saveDir.exists()) {
        bool ok = saveDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, vehicle_name);

    settings->saveJson(filename);

}

void FormGPS::vehicle_load(QString vehicle_name) {
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                            + "/" + QCoreApplication::applicationName() + "/Vehicles";

    QDir loadDir(directoryName);
    if (!loadDir.exists()) {
        bool ok = loadDir.mkpath(directoryName);
        if (!ok) {
            qWarning() << "Couldn't create path " << directoryName;
            return;
        }
    }

    if (!loadDir.exists(caseInsensitiveFilename(directoryName, vehicle_name)))
        qWarning() << vehicle_name << " may not exist but will try to load it anyway.";

    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, vehicle_name);

    settings->loadJson(filename);
}

void FormGPS::vehicle_delete(QString vehicle_name) {
    /*
    if ((bool)property_setMenu_isMetric) {
        settings->setValue("display/isMetric", "false");
        qml_settings.insert("testing123","false");
    } else {
        settings->setValue("display/isMetric", "true");
        qml_settings.insert("testing123","true");
    }

    return;
    */

    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                            + "/" + QCoreApplication::applicationName() + "/Vehicles";
    QString filename = directoryName + "/" + caseInsensitiveFilename(directoryName, vehicle_name);

    QDir vehicleDir(directoryName);
    if (vehicleDir.exists()) {
        if (! vehicleDir.remove(caseInsensitiveFilename(directoryName, vehicle_name)))
            qWarning() << "Could not delete vehicle " << vehicle_name;
    }
}

void FormGPS::vehicle_update_list() {
    QObject *vehicleInterface;

    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                            + "/" + QCoreApplication::applicationName() + "/Vehicles";

    vehicleInterface = qmlItem(qml_root, "vehicleInterface");

    QDir vehicleDirectory(directoryName);
    if(!vehicleDirectory.exists()) {
        vehicleDirectory.mkpath(directoryName);
    }

    vehicleDirectory.setFilter(QDir::Files);

    QFileInfoList filesList = vehicleDirectory.entryInfoList();

    QList<QVariant> vehicleList;
    QMap<QString, QVariant>vehicle;
    int index = 0;

    for (QFileInfo file : filesList) {
        vehicle.clear();
        vehicle["index"] = index;
        vehicle["name"] = file.fileName();
        vehicleList.append(vehicle);
        index++;
    }

    vehicleInterface->setProperty("vehicle_list", vehicleList);

}

