#include "formgps.h"
#include "qmlutil.h"
#include "aogproperty.h"

void FormGPS::update_current_ABline_from_qml()
{
    //AOGInterface currentABLine property changed; sync our
    //local ABLine.numABLineSelected with it.

    QObject *aog = qmlItem(qml_root, "aog"); //TODO save this in formgps.h

    //the property will be -1 if nothing is selected, ABLine uses base 1
    //so add one to it
    ABLine.numABLineSelected = aog->property("currentABLine").toInt() + 1;
    if (ABLine.numABLineSelected > ABLine.lineArr.count())
        ABLine.numABLineSelected = 0;

    ABLine.isABValid = false; //recalculate the closest line to us
    ABLine.moveDistance = 0;

    if (ABLine.numABLineSelected > 0) {
        ABLine.abHeading = ABLine.lineArr[ABLine.numABLineSelected-1].heading;
        ABLine.refPoint1 = ABLine.lineArr[ABLine.numABLineSelected-1].origin;
        ABLine.SetABLineByHeading();
        ABLine.isBtnABLineOn = true;
        ABLine.isABLineSet = true;
        ABLine.isABLineLoaded = true;
        ABLine.isABLineBeingSet = false;
        aog->setProperty("currentABLine_heading", ABLine.abHeading);
    } else {
        ABLine.isBtnABLineOn = false;
        ABLine.isABLineSet = false;
        ABLine.isABLineLoaded = false;
        ABLine.isABLineBeingSet = false;
    }

    int selectedItem = aog->property("currentABCurve").toInt();
    //reset to generate new reference
    curve.isCurveValid = false;
    curve.moveDistance = 0;
    curve.desList.clear();

    FileSaveCurveLines(); // in case a new one was added

    if (selectedItem > -1 && selectedItem <= curve.curveArr.count())
    {
        int idx = selectedItem;
        curve.numCurveLineSelected = idx + 1;

        curve.aveLineHeading = curve.curveArr[idx].aveHeading;
        curve.refList.clear();
        for (int i = 0; i < curve.curveArr[idx].curvePts.count(); i++)
        {
            curve.refList.append(curve.curveArr[idx].curvePts[i]);
        }
        curve.isCurveSet = true;
        yt.ResetYouTurn();
    }
    else
    {
        curve.numCurveLineSelected = 0;
        curve.isOkToAddDesPoints = false;
        curve.isCurveSet = false;
        curve.refList.clear();
        curve.isCurveSet = false;
        //DisableYouTurnButtons();
        //done in QML
        curve.isBtnCurveOn = false;
    }

    if (ABLine.numABLineSelected == 0 && curve.numCurveLineSelected == 0 && ct.isContourBtnOn == false) {
        isAutoSteerBtnOn = false;
    }
}

void FormGPS::update_ABlines_in_qml()
{
    QObject *linesInterface = qmlItem(qml_root,"linesInterface");

    QList<QVariant> list;
    QMap<QString, QVariant>line;

    for(int i=0; i < ABLine.lineArr.count(); i++) {
        line.clear();
        line["index"] = i;
        line["name"] = ABLine.lineArr[i].Name;
        line["easting"] = ABLine.lineArr[i].origin.easting;
        line["northing"] = ABLine.lineArr[i].origin.northing;
        line["heading"] = ABLine.lineArr[i].heading;
        line["visible"] = ABLine.lineArr[i].isVisible;
        list.append(line);
    }

    linesInterface->setProperty("abLinesList",list);

    list.clear();
    for(int i=0; i < curve.curveArr.count(); i++) {
        line.clear();
        line["index"] = i;
        line["name"] = curve.curveArr[i].Name;
        line["visible"] = curve.curveArr[i].isVisible;
        list.append(line);
    }

    linesInterface->setProperty("abCurvesList",list);
}

void FormGPS::add_new_ABline(QString name, double easting, double northing, double heading)
{
    qDebug() << name << easting << northing << heading;
    CABLines line;
    line.origin = Vec2(easting, northing);
    line.heading = heading;
    line.Name = name;
    ABLine.lineArr.append(line);
    FileSaveABLines();
    update_ABlines_in_qml();
}

void FormGPS::start_newABLine(bool start_or_cancel, double easting, double northing, double heading)
{
    if (!start_or_cancel) {
        ABLine.isABLineBeingSet = false;
        return;
    }

    ABLine.desPoint1.easting = easting + cos(heading) * (double)property_setVehicle_toolOffset;
    ABLine.desPoint2.northing = northing + sin(heading) * (double)property_setVehicle_toolOffset;

    ABLine.desHeading = heading;

    ABLine.desPoint2.easting = 99999;
    ABLine.desPoint2.northing = 99999;

    ABLine.isABLineBeingSet = true;

    ABLine.desHeading = heading;
    ABLine.desP1.easting = ABLine.desPoint1.easting - (sin(ABLine.desHeading) * (double)property_setAB_lineLength);
    ABLine.desP1.northing = ABLine.desPoint1.northing - (cos(ABLine.desHeading) * (double)property_setAB_lineLength);
    ABLine.desP2.easting = ABLine.desPoint1.easting + (sin(ABLine.desHeading) * (double)property_setAB_lineLength);
    ABLine.desP2.northing = ABLine.desPoint1.northing + (cos(ABLine.desHeading) * (double)property_setAB_lineLength);
}

void FormGPS::delete_ABLine(int which_one)
{
    QObject *linesInterface = qmlItem(qml_root,"linesInterface");

    if ((which_one < 0) || (which_one >= ABLine.lineArr.count()))
        return;

    ABLine.lineArr.removeAt(which_one);

    linesInterface->setProperty("currentABLine", -1);

    update_ABlines_in_qml();
    update_current_ABline_from_qml();
    FileSaveABLines();
}

void FormGPS::swap_heading_ABLine(int which_one)
{
    if ((which_one < 0) || (which_one >= ABLine.lineArr.count()))
        return;

    double heading = ABLine.lineArr[which_one].heading + M_PI;
    if (heading > glm::twoPI) //if over 360
        heading -= glm::twoPI;

    ABLine.lineArr[which_one].heading = heading;
    update_ABlines_in_qml();
    FileSaveABLines();
}

void FormGPS::change_name_ABLine(int which_one, QString name)
{
    qDebug() << "changing name of " << which_one << " to " << name;
    if (which_one > -1) {
        ABLine.lineArr[which_one].Name = name;
        update_ABlines_in_qml();
        FileSaveABLines();
    }
}

