// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// GUI to backend tracks interface
#include "formgps.h"
#include "qmlutil.h"
#include "aogproperty.h"


void FormGPS::tracks_select(int index)
{
    //reset to generate new reference
    curve.isCurveValid = false;
    curve.lastHowManyPathsAway = 98888;
    ABLine.isABValid = false;
    curve.desList.clear();

    FileSaveTracks();

    //We assume that QML will always pass us a valid index that is
    //visible, or -1
    trk.setIdx(index);
    yt.ResetCreatedYouTurn(makeUTurnCounter);
}

void FormGPS::tracks_start_new(int mode, double easting, double northing, double heading)
{
    switch(mode) {
    case TrackMode::AB:
        curve.desList.clear();
        ABLine.isMakingABLine = true;
        ABLine.desPtA.easting = easting;
        ABLine.desPtA.northing = northing;

        ABLine.desLineEndA.easting = ABLine.desPtA.easting - (sin(vehicle.pivotAxlePos.heading) * 1000);
        ABLine.desLineEndA.northing = ABLine.desPtA.northing - (cos(vehicle.pivotAxlePos.heading) * 1000);

        ABLine.desLineEndB.easting = ABLine.desPtA.easting + (sin(vehicle.pivotAxlePos.heading) * 1000);
        ABLine.desLineEndB.northing = ABLine.desPtA.northing + (cos(vehicle.pivotAxlePos.heading) * 1000);

        break;

    case TrackMode::Curve:
        curve.desList.clear();
        curve.isMakingCurve = true;
        break;

    case TrackMode::waterPivot:
        //nothing to do here.
    default:
        return;
    }
}

void FormGPS::tracks_finish_new(int mode, QString name, int ref_side, double easting, double northing, double heading)
{
    double aveLineHeading = 0;
    int idx;
    int cnt;

    switch(mode) {
    case TrackMode::AB:
       break;

    case TrackMode::Curve:
        curve.isMakingCurve = false;

        cnt = curve.desList.count();
        if (cnt > 3)
        {
            //make sure point distance isn't too big
            curve.MakePointMinimumSpacing(curve.desList, 1.6);
            curve.CalculateHeadings(curve.desList);

            trk.gArr.append(CTrk());
            //array number is 1 less since it starts at zero
            idx = trk.gArr.count() - 1;

            trk.gArr[idx].ptA = Vec2(curve.desList[0].easting,
                                     curve.desList[0].northing);
            trk.gArr[idx].ptB = Vec2(curve.desList[curve.desList.count() - 1].easting,
                                     curve.desList[curve.desList.count() - 1].northing);

            trk.gArr[idx].mode = TrackMode::Curve;

            //calculate average heading of line
            double x = 0, y = 0;
            for (Vec3 &pt : curve.desList)
            {
                x += cos(pt.heading);
                y += sin(pt.heading);
            }
            x /= curve.desList.count();
            y /= curve.desList.count();
            aveLineHeading = atan2(y, x);
            if (aveLineHeading < 0) aveLineHeading += glm::twoPI;

            trk.gArr[idx].heading = aveLineHeading;

            //build the tail extensions
            curve.AddFirstLastPoints(curve.desList,bnd);
            curve.SmoothABDesList(4);
            curve.CalculateHeadings(curve.desList);

            //write out the Curve Points
            for (Vec3 &item : curve.desList)
            {
                trk.gArr[idx].curvePts.append(item);
            }

            curve.desName = "Cu " + locale.toString(glm::toDegrees(aveLineHeading), 'g', 1) + QChar(0x00B0);

            double dist;

            if (ref_side > 0)
            {
                dist = (tool.width - tool.overlap) * 0.5 + tool.offset;
                trk.NudgeRefCurve(dist,curve);
            }
            else if (ref_side < 0)
            {
                dist = (tool.width - tool.overlap) * -0.5 + tool.offset;
                trk.NudgeRefCurve(dist,curve);
            }
            //else no nudge, center ref line

            trk.setIdx(idx);
        }
        else
        {
            curve.isMakingCurve = false;
            curve.desList.clear();
        }

         break;

    case TrackMode::waterPivot:
        break;

    default:
        return;

    }

}
    /*
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

    if (ABLine.numABLineSelected == 0 && curve.numCurveLineSelected == 0 && (bool)ct.isContourBtnOn == false) {
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
*/
