#include "formgps.h"
#include "qmlutil.h"
#include "aogproperty.h"


void FormGPS::field_update_list() {
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                            + "/" + QCoreApplication::applicationName() + "/Fields";

    QObject *fieldInterface = qmlItem(qml_root, "fieldInterface");

    QDir fieldsDirectory(directoryName);
    fieldsDirectory.setFilter(QDir::Dirs);

    QList<QVariant> fieldList;
    QMap<QString, QVariant> field;

    QFileInfoList fieldsDirList = fieldsDirectory.entryInfoList();

    int index = 0;

    for (QFileInfo fieldDir : fieldsDirList) {

        if(fieldDir.fileName() == "." ||
            fieldDir.fileName() == "..")
            continue;
        field = FileFieldInfo(fieldDir.fileName());

        if(field.contains("latitude")) {
            field["index"] = index;
            fieldList.append(field);
            index++;
        }

        field["name"] = fieldDir.fileName(); // in case Field.txt doesn't agree with dir name
    }

    fieldInterface->setProperty("field_list", fieldList);
}

void FormGPS::field_close() {
    lock.lockForWrite();
    fileSaveEverythingBeforeClosingField();
    lock.unlock();
}

void FormGPS::field_open(QString field_name) {
    lock.lockForWrite();
    fileSaveEverythingBeforeClosingField();
    if (! FileOpenField(field_name)) {
        TimedMessageBox(8000, tr("Saved field does not exist."), QString(tr("Cannot find the requested saved field.")) + " " +
                                                                field_name);

        property_setF_CurrentDir = "Default";
    }
    lock.unlock();
}

void FormGPS::field_new(QString field_name) {
    //assume the GUI will vet the name a little bit
    lock.lockForWrite();
    currentFieldDirectory = field_name;
    property_setF_CurrentDir = currentFieldDirectory;
    JobNew();

    pn.latStart = pn.latitude;
    pn.lonStart = pn.longitude;
    pn.SetLocalMetersPerDegree();

    FileCreateField();
    FileCreateSections();
    FileCreateRecPath();
    FileCreateContour();
    FileCreateElevation();
    FileSaveFlags();
    FileCreateBoundary();
    FileSaveTram();
    lock.unlock();
}

void FormGPS::field_new_from(QString existing, QString field_name, int flags) {
    lock.lockForWrite();
    fileSaveEverythingBeforeClosingField();
    if (! FileOpenField(existing,flags)) { //load whatever is requested from existing field
        TimedMessageBox(8000, tr("Existing field cannot be found"), QString(tr("Cannot find the existing saved field.")) + " " +
                                                                existing);
    }
    //change to new name
    currentFieldDirectory = field_name;
    property_setF_CurrentDir = currentFieldDirectory;

    FileCreateField();
    FileCreateSections();
    FileCreateElevation();
    FileSaveFlags();
    FileSaveABLines();
    FileSaveCurveLines();

    contourSaveList.clear();
    contourSaveList.append(ct.ptList);
    FileSaveContour();

    FileSaveRecPath();
    FileSaveTram();

    //some how we have to write the existing patches to the disk.
    //FileSaveSections only write pending triangles

    for(QSharedPointer<PatchTriangleList> l: triStrip[0].patchList) {
        tool.patchSaveList.append(l);
    }
    FileSaveSections();
    lock.unlock();
}

void FormGPS::field_delete(QString field_name) {
    QString directoryName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                            + "/" + QCoreApplication::applicationName() + "/Fields/" + field_name;

    QDir fieldDir(directoryName);

    if(! fieldDir.exists()) {
        TimedMessageBox(8000,tr("Cannot find saved field"),QString(tr("Cannot find saved field to delete.")) + " " + field_name);
        return;
    }

    QFile::moveToTrash(directoryName);
    field_update_list();
}
