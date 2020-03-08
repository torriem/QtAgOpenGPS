#include "ccontour.h"
#include "cvehicle.h"
#include "cvec.h"
#include <math.h>
#include <limits>
#include <QOpenGLFunctions>
#include "glm.h"
#include "cvehicle.h"
#include <QSettings>
#include "glutils.h"
#include "cboundary.h"
#include "ctool.h"
#include "aogsettings.h"
#include "cnmea.h"
#include "common.h"

CContour::CContour(QObject *parent)
    : QObject(parent), ptList(new QVector<Vec3>)
{

}

//start stop and add points to list
void CContour::startContourLine(Vec3 pivot) {
    qDebug() << "starting contour line.";
    isContourOn = true;
    if (!ptList.isNull() && ptList->size() == 1)
    {
        //reuse ptList
        ptList->clear();
    }
    else
    {
        //make new ptList
        ptList = QSharedPointer<QVector<Vec3>>(new QVector<Vec3>);
        stripList.append(ptList);
    }

    ptList->append(pivot);
}

//Add current position to stripList
void CContour::addPoint(Vec3 pivot) {
    ptList->append(pivot);
}

//End the strip
void CContour::stopContourLine(Vec3 pivot, QVector<QSharedPointer<QVector<Vec3>>> &contourSaveList)
{
    qDebug() << ptList->size() << "Stopping contour line.";
    //make sure its long enough to bother
    if (ptList->size() > 10)
    {
        ptList->append(pivot);

        //add the point list to the save list for appending to contour file
        contourSaveList.append(ptList);
    }

    //delete ptList
    else
    {
        ptList->clear();
        stripList.pop_back(); //remove the last list in the list
    }

    //turn it off
    isContourOn = false;
}

void CContour::buildBoundaryContours(CTool &tool, CBoundary &bnd, int pass, int spacingInt)
{
    if (bnd.bndArr.size() == 0)
    {
        qDebug() << "Boundary Contour Error, No Boundaries Made";
        //emit signal
        //mf.TimedMessageBox(1500, "Boundary Contour Error", "No Boundaries Made");
        emit showMessage(messageBox::error, tr("Boundary Contour Error"), tr("No Boundaries Made"));
        return;
    }

    //convert to meters
    double spacing = spacingInt;
    spacing *= 0.01;

    Vec3 point;
    double totalHeadWidth = 0;
    int signPass = -1;

    if (pass == 1)
    {
        signPass = -1;
        //determine how wide a headland space
        totalHeadWidth = ((tool.toolWidth - tool.toolOverlap) * 0.5) - spacing;
    }

    else
    {
        signPass = 1;
        totalHeadWidth = ((tool.toolWidth - tool.toolOverlap) * pass) + spacing +
            ((tool.toolWidth - tool.toolOverlap) * 0.5);
    }

    //outside boundary

    //count the points from the boundary
    int ptCount = bnd.bndArr[0].bndLine.size();

    ptList = QSharedPointer<QVector<Vec3>>(new QVector<Vec3>);
    stripList.append(ptList);

    for (int i = ptCount - 1; i >= 0; i--)
    {
        //calculate the point inside the boundary
        point.easting = bnd.bndArr[0].bndLine[i].easting - (signPass * sin(glm::PIBy2 + bnd.bndArr[0].bndLine[i].heading) * totalHeadWidth);
        point.northing = bnd.bndArr[0].bndLine[i].northing - (signPass * cos(glm::PIBy2 + bnd.bndArr[0].bndLine[i].heading) * totalHeadWidth);
        point.heading = bnd.bndArr[0].bndLine[i].heading - M_PI;
        if (point.heading < -glm::twoPI) point.heading += glm::twoPI;
        ptList->append(point);
    }

    //totalHeadWidth = (tool.toolWidth - tool.toolOverlap) * 0.5 + 0.2 + (tool.toolWidth - tool.toolOverlap);

    for (int j = 1; j < bnd.bndArr.size(); j++)
    {
        if (!bnd.bndArr[j].isSet) continue;

        //count the points from the boundary
        ptCount = bnd.bndArr[j].bndLine.size();

        ptList = QSharedPointer<QVector<Vec3>>(new QVector<Vec3>);
        stripList.append(ptList);

        for (int i = ptCount - 1; i >= 0; i--)
        {
            //calculate the point inside the boundary
            point.easting = bnd.bndArr[j].bndLine[i].easting - (signPass * sin(glm::PIBy2 + bnd.bndArr[j].bndLine[i].heading) * totalHeadWidth);
            point.northing = bnd.bndArr[j].bndLine[i].northing - (signPass * cos(glm::PIBy2 + bnd.bndArr[j].bndLine[i].heading) * totalHeadWidth);
            point.heading = bnd.bndArr[j].bndLine[i].heading - M_PI;
            if (point.heading < -glm::twoPI) point.heading += glm::twoPI;

            //only add if inside actual field boundary
            ptList->append(point);
        }

        //add the point list to the save list for appending to contour file
        //mf.contourSaveList.append(ptList);
    }

    qDebug() << "Boundary Contour Contour Path Created";

    //emit signal
    //mf.TimedMessageBox(1500, "Boundary Contour", "Contour Path Created");
    emit showMessage(messageBox::info, tr("Boundary Contour"),tr("Contour Path Created"));

}

//determine closest point on applied
void CContour::buildContourGuidanceLine(CVehicle &vehicle, CTool& tool, CNMEA &pn, Vec3 pivot)
{
    double toolWid = tool.toolWidth;

    double sinH = sin(pivot.heading) * 2.0 * toolWid;
    double cosH = cos(pivot.heading) * 2.0 * toolWid;

    double sin2HL = 0;
    double cos2HL = 0;
    double sin2HR = 0;
    double cos2HR = 0;

    if (tool.toolOffset < 0)
    {
        //sticks out more left
        sin2HL = sin(pivot.heading + glm::PIBy2) * (1.33 * (toolWid + fabs(tool.toolOffset * 2)));
        cos2HL = cos(pivot.heading + glm::PIBy2) * (1.33 * (toolWid + fabs(tool.toolOffset * 2)));

        sin2HR = sin(pivot.heading + glm::PIBy2) * (1.33 * (toolWid + fabs(tool.toolOffset)));
        cos2HR = cos(pivot.heading + glm::PIBy2) * (1.33 * (toolWid + fabs(tool.toolOffset)));
    }
    else
    {
        //sticks out more right
        sin2HL = sin(pivot.heading + glm::PIBy2) * (1.33 * (toolWid + fabs(tool.toolOffset)));
        cos2HL = cos(pivot.heading + glm::PIBy2) * (1.33 * (toolWid + fabs(tool.toolOffset)));

        sin2HR = sin(pivot.heading + glm::PIBy2) * (1.33 * (toolWid + fabs(tool.toolOffset * 2)));
        cos2HR = cos(pivot.heading + glm::PIBy2) * (1.33 * (toolWid + fabs(tool.toolOffset * 2)));
    }

    //narrow equipment needs bigger bounding box.
    if (tool.toolWidth < 6)
    {
        sinH = sin(pivot.heading) * 4 * toolWid;
        cosH = cos(pivot.heading) * 4 * toolWid;
    }

    double sin3H = sin(pivot.heading + glm::PIBy2) * 0.5;
    double cos3H = cos(pivot.heading + glm::PIBy2) * 0.5;

    //build a frustum box ahead of fix to find adjacent paths and points

    //left
    boxA.easting = pivot.easting - sin2HL;
    boxA.northing = pivot.northing - cos2HL;
    boxA.easting -= (sinH * 0.25); //bottom left outside
    boxA.northing -= (cosH * 0.25);

    boxD.easting = boxA.easting + sinH; //top left outside
    boxD.northing = boxA.northing + cosH;

    boxE.easting = pivot.easting - sin3H; // inside bottom
    boxE.northing = pivot.northing - cos3H;

    boxG.easting = boxE.easting + (sinH * 0.3); //inside top
    boxG.northing = boxE.northing + (cosH * 0.3);

    //right
    boxB.easting = pivot.easting + sin2HR;
    boxB.northing = pivot.northing + cos2HR;
    boxB.easting -= (sinH * 0.25);
    boxB.northing -= (cosH * 0.25);

    boxC.easting = boxB.easting + sinH;
    boxC.northing = boxB.northing + cosH;

    boxF.easting = pivot.easting + sin3H;
    boxF.northing = pivot.northing + cos3H;

    boxH.easting = boxF.easting + (sinH * 0.3); //inside top
    boxH.northing = boxF.northing + (cosH * 0.3);

    conList.clear();
    ctList.clear();
    int ptCount;

    //check if no strips yet, return
    int stripCount = stripList.size();
    if (stripCount == 0) return;

    CVec pointC;
    if (isRightPriority)
    {
        //determine if points are in right side frustum box
        for (int s = 0; s < stripCount; s++)
        {
            ptCount = stripList[s]->size();
            for (int p = 0; p < ptCount; p++)
            {
                //FHCBF
                if ((((boxH.easting - boxC.easting) * ((*stripList[s])[p].northing - boxC.northing))
                        - ((boxH.northing - boxC.northing) * ((*stripList[s])[p].easting - boxC.easting))) < 0) { continue; }

                if ((((boxC.easting - boxB.easting) * ((*stripList[s])[p].northing - boxB.northing))
                        - ((boxC.northing - boxB.northing) * ((*stripList[s])[p].easting - boxB.easting))) < 0) { continue; }

                if ((((boxB.easting - boxF.easting) * ((*stripList[s])[p].northing - boxF.northing))
                        - ((boxB.northing - boxF.northing) * ((*stripList[s])[p].easting - boxF.easting))) < 0) { continue; }

                if ((((boxF.easting - boxH.easting) * ((*stripList[s])[p].northing - boxH.northing))
                        - ((boxF.northing - boxH.northing) * ((*stripList[s])[p].easting - boxH.easting))) < 0) { continue; }

                //in the box so is it parallelish or perpedicularish to current heading
                ref2 = M_PI - fabs(fabs(vehicle.fixHeading - (*stripList[s])[p].heading) - M_PI);
                if (ref2 < 1.2 || ref2 > 1.9)
                {
                    //it's in the box and parallelish so add to list
                    pointC.x = (*stripList[s])[p].easting;
                    pointC.z = (*stripList[s])[p].northing;
                    pointC.h = (*stripList[s])[p].heading;
                    pointC.strip = s;
                    pointC.pt = p;
                    conList.append(pointC);
                }
            }
        }

        if (conList.size() == 0)
        {
            //determine if points are in frustum box
            for (int s = 0; s < stripCount; s++)
            {
                ptCount = stripList[s]->size();
                for (int p = 0; p < ptCount; p++)
                {
                    //EADGE
                    if ((((boxG.easting - boxE.easting) * ((*stripList[s])[p].northing - boxE.northing))
                            - ((boxG.northing - boxE.northing) * ((*stripList[s])[p].easting - boxE.easting))) < 0) { continue; }

                    if ((((boxE.easting - boxA.easting) * ((*stripList[s])[p].northing - boxA.northing))
                            - ((boxE.northing - boxA.northing) * ((*stripList[s])[p].easting - boxA.easting))) < 0) { continue; }

                    if ((((boxA.easting - boxD.easting) * ((*stripList[s])[p].northing - boxD.northing))
                            - ((boxA.northing - boxD.northing) * ((*stripList[s])[p].easting - boxD.easting))) < 0) { continue; }

                    if ((((boxD.easting - boxG.easting) * ((*stripList[s])[p].northing - boxG.northing))
                            - ((boxD.northing - boxG.northing) * ((*stripList[s])[p].easting - boxG.easting))) < 0) { continue; }

                    //in the box so is it parallelish or perpedicularish to current heading
                    ref2 = M_PI - fabs(fabs(vehicle.fixHeading - (*stripList[s])[p].heading) - M_PI);
                    if (ref2 < 1.2 || ref2 > 1.9)
                    {
                        //it's in the box and parallelish so add to list
                        pointC.x = (*stripList[s])[p].easting;
                        pointC.z = (*stripList[s])[p].northing;
                        pointC.h = (*stripList[s])[p].heading;
                        pointC.strip = s;
                        pointC.pt = p;
                        conList.append(pointC);
                    }
                }
            }
        }
    }
    else
    {
        for (int s = 0; s < stripCount; s++)
        {
            ptCount = stripList[s]->size();
            for (int p = 0; p < ptCount; p++)
            {
                //EADGE
                if ((((boxG.easting - boxE.easting) * ((*stripList[s])[p].northing - boxE.northing))
                        - ((boxG.northing - boxE.northing) * ((*stripList[s])[p].easting - boxE.easting))) < 0) { continue; }

                if ((((boxE.easting - boxA.easting) * ((*stripList[s])[p].northing - boxA.northing))
                        - ((boxE.northing - boxA.northing) * ((*stripList[s])[p].easting - boxA.easting))) < 0) { continue; }

                if ((((boxA.easting - boxD.easting) * ((*stripList[s])[p].northing - boxD.northing))
                        - ((boxA.northing - boxD.northing) * ((*stripList[s])[p].easting - boxD.easting))) < 0) { continue; }

                if ((((boxD.easting - boxG.easting) * ((*stripList[s])[p].northing - boxG.northing))
                        - ((boxD.northing - boxG.northing) * ((*stripList[s])[p].easting - boxG.easting))) < 0) { continue; }

                //in the box so is it parallelish or perpedicularish to current heading
                ref2 = M_PI - fabs(fabs(vehicle.fixHeading - (*stripList[s])[p].heading) - M_PI);
                if (ref2 < 1.2 || ref2 > 1.9)
                {
                    //it's in the box and parallelish so add to list
                    pointC.x = (*stripList[s])[p].easting;
                    pointC.z = (*stripList[s])[p].northing;
                    pointC.h = (*stripList[s])[p].heading;
                    pointC.strip = s;
                    pointC.pt = p;
                    conList.append(pointC);
                }
            }
        }

        if (conList.size() == 0)
        {
            //determine if points are in frustum box
            for (int s = 0; s < stripCount; s++)
            {
                ptCount = stripList[s]->size();
                for (int p = 0; p < ptCount; p++)
                {
                    if ((((boxH.easting - boxC.easting) * ((*stripList[s])[p].northing - boxC.northing))
                            - ((boxH.northing - boxC.northing) * ((*stripList[s])[p].easting - boxC.easting))) < 0) { continue; }

                    if ((((boxC.easting - boxB.easting) * ((*stripList[s])[p].northing - boxB.northing))
                            - ((boxC.northing - boxB.northing) * ((*stripList[s])[p].easting - boxB.easting))) < 0) { continue; }

                    if ((((boxB.easting - boxF.easting) * ((*stripList[s])[p].northing - boxF.northing))
                            - ((boxB.northing - boxF.northing) * ((*stripList[s])[p].easting - boxF.easting))) < 0) { continue; }

                    if ((((boxF.easting - boxH.easting) * ((*stripList[s])[p].northing - boxH.northing))
                            - ((boxF.northing - boxH.northing) * ((*stripList[s])[p].easting - boxH.easting))) < 0) { continue; }

                    //in the box so is it parallelish or perpedicularish to current heading
                    ref2 = M_PI - fabs(fabs(vehicle.fixHeading - (*stripList[s])[p].heading) - M_PI);
                    if (ref2 < 1.2 || ref2 > 1.9)
                    {
                        //it's in the box and parallelish so add to list
                        pointC.x = (*stripList[s])[p].easting;
                        pointC.z = (*stripList[s])[p].northing;
                        pointC.h = (*stripList[s])[p].heading;
                        pointC.strip = s;
                        pointC.pt = p;
                        conList.append(pointC);
                    }
                }
            }
        }
    }

    //no points in the box, exit
    ptCount = conList.size();
    if (ptCount == 0)
    {
        distanceFromCurrentLine = 9999;
        distanceFromCurrentLine = 32000;
        vehicle.guidanceLineDistanceOff = 32000;
        return;
    }

    //determine closest point
    minDistance = 99999;
    for (int i = 0; i < ptCount; i++)
    {
        double dist = ((pivot.easting - conList[i].x) * (pivot.easting - conList[i].x))
                        + ((pivot.northing - conList[i].z) * (pivot.northing - conList[i].z));
        if (minDistance >= dist)
        {
            minDistance = dist;
            closestRefPoint = i;
        }
    }

    //now we have closest point, the distance squared from it, and which patch and point its from
    int strip = conList[closestRefPoint].strip;
    int pt = conList[closestRefPoint].pt;
    refX = (*stripList[strip])[pt].easting;
    refZ = (*stripList[strip])[pt].northing;
    refHeading = (*stripList[strip])[pt].heading;

    //are we going same direction as stripList was created?
    bool isSameWay = M_PI - fabs(fabs(vehicle.fixHeading - refHeading) - M_PI) < 1.4;

    //which side of the patch are we on is next
    //calculate endpoints of reference line based on closest point
    refPoint1.easting = refX - (sin(refHeading) * 50.0);
    refPoint1.northing = refZ - (cos(refHeading) * 50.0);

    refPoint2.easting = refX + (sin(refHeading) * 50.0);
    refPoint2.northing = refZ + (cos(refHeading) * 50.0);

    //x2-x1
    double dx = refPoint2.easting - refPoint1.easting;
    //z2-z1
    double dz = refPoint2.northing - refPoint1.northing;

    //how far are we away from the reference line at 90 degrees - 2D cross product and distance
    distanceFromRefLine = ((dz * pn.fix.easting) - (dx * pn.fix.northing) + (refPoint2.easting
                            * refPoint1.northing) - (refPoint2.northing * refPoint1.easting))
                                / sqrt((dz * dz) + (dx * dx));

    //add or subtract pi by 2 depending on which side of ref line
    double piSide;

    //sign of distance determines which side of line we are on
    if (distanceFromRefLine > 0) piSide = glm::PIBy2;
    else piSide = -glm::PIBy2;

    //offset calcs
    double toolOffset = tool.toolOffset;
    if (isSameWay)
    {
        toolOffset = 0.0;
    }
    else
    {
        if (distanceFromRefLine > 0) toolOffset *= 2.0;
        else toolOffset *= -2.0;
    }

    //move the Guidance Line over based on the overlap, width, and offset amount set in vehicle
    double widthMinusOverlap = tool.toolWidth - tool.toolOverlap + toolOffset;

    //absolute the distance
    distanceFromRefLine = fabs(distanceFromRefLine);

    //make the new guidance line list called guideList
    ptCount = stripList[strip]->size() - 1;
    int start, stop;

    start = pt - 35; if (start < 0) start = 0;
    stop = pt + 35; if (stop > ptCount) stop = ptCount + 1;

    double distSq = widthMinusOverlap * widthMinusOverlap * 0.95;
    bool fail = false;

    for (int i = start; i < stop; i++)
    {
        //var point = new vec3(
        //    (*stripList[strip])[i].easting + (sin(piSide + (*stripList[strip])[i].heading) * widthMinusOverlap),
        //    (*stripList[strip])[i].northing + (cos(piSide + (*stripList[strip])[i].heading) * widthMinusOverlap),
        //    (*stripList[strip])[i].heading);
        //ctList.append(point);

        Vec3 point = Vec3(
            (*stripList[strip])[i].easting + (sin(piSide + (*stripList[strip])[i].heading) * widthMinusOverlap),
            (*stripList[strip])[i].northing + (cos(piSide + (*stripList[strip])[i].heading) * widthMinusOverlap),
            (*stripList[strip])[i].heading);
        //ctList.append(point);

        //make sure its not closer then 1 eq width
        for (int j = start; j < stop; j++)
        {
            double check = glm::distanceSquared(point.northing, point.easting, (*stripList[strip])[j].northing, (*stripList[strip])[j].easting);
            if (check < distSq)
            {
                fail = true;
                break;
            }
        }

        if (!fail) ctList.append(point);
        fail = false;
    }

    int ctCount = ctList.size();
    if (ctCount < 6) return;

    const double spacing = 1.0;
    double distance;
    for (int i = 0; i < ctCount - 1; i++)
    {
        distance = glm::distance(ctList[i], ctList[i + 1]);
        if (distance < spacing)
        {
            ctList.removeAt(i + 1);
            ctCount = ctList.size();
            i--;
        }
    }
}

//determine distance from contour guidance line
void CContour::distanceFromContourLine(CVehicle &vehicle, CNMEA &pn, Vec3 pivot, Vec3 steer)
{
    isValid = false;
    double minDistA = 1000000, minDistB = 1000000;
    int ptCount = ctList.size();
    //distanceFromCurrentLine = 9999;
    if (ptCount > 8)
    {
        if (vehicle.isStanleyUsed)
        {
            //find the closest 2 points to current fix
            for (int t = 0; t < ptCount; t++)
            {
                double dist = ((steer.easting - ctList[t].easting) * (steer.easting - ctList[t].easting))
                                + ((steer.northing - ctList[t].northing) * (steer.northing - ctList[t].northing));
                if (dist < minDistA)
                {
                    minDistB = minDistA;
                    B = A;
                    minDistA = dist;
                    A = t;
                }
                else if (dist < minDistB)
                {
                    minDistB = dist;
                    B = t;
                }
            }

            //just need to make sure the points continue ascending in list order or heading switches all over the place
            if (A > B) { C = A; A = B; B = C; }

            //get the distance from currently active AB line
            //x2-x1
            double dx = ctList[B].easting - ctList[A].easting;
            //z2-z1
            double dy = ctList[B].northing - ctList[A].northing;

            if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dy) < glm::DOUBLE_EPSILON) return;

            abHeading = atan2(dx, dy);
            if (abHeading < 0) abHeading += glm::twoPI;
            //if (abHeading > M_PI) abHeading -= glm::twoPI;

            //abHeading = ctList[A].heading;

            //how far from current AB Line is fix
            distanceFromCurrentLine = ((dy * steer.easting) - (dx * steer.northing) + (ctList[B].easting
                        * ctList[A].northing) - (ctList[B].northing * ctList[A].easting))
                            / sqrt((dy * dy) + (dx * dx));

            //are we on the right side or not
            isOnRightSideCurrentLine = distanceFromCurrentLine > 0;

            //absolute the distance
            distanceFromCurrentLine = fabs(distanceFromCurrentLine);

            //Subtract the two headings, if > 1.57 its going the opposite heading as refAB
            abFixHeadingDelta = (fabs(vehicle.fixHeading - abHeading));
            if (abFixHeadingDelta >= M_PI) abFixHeadingDelta = fabs(abFixHeadingDelta - glm::twoPI);

            isABSameAsVehicleHeading = abFixHeadingDelta < glm::PIBy2;

            // calc point on ABLine closest to current position
            double U = (((steer.easting - ctList[A].easting) * dx) + ((steer.northing - ctList[A].northing) * dy))
                        / ((dx * dx) + (dy * dy));

            rEastCT = ctList[A].easting + (U * dx);
            rNorthCT = ctList[A].northing + (U * dy);

            ////find closest point to goal to get heading.
            //minDistA = 99999;
            //for (int t = 0; t < ptCount; t++)
            //{
            //    double dist = ((rEastCT - ctList[t].easting) * (rEastCT - ctList[t].easting))
            //                    + ((rNorthCT - ctList[t].northing) * (rNorthCT - ctList[t].northing));
            //    if (dist < minDistA)
            //    {
            //        A = t;
            //    }
            //}

            //abHeading = ctList[A].heading;

            //distance is negative if on left, positive if on right
            if (isABSameAsVehicleHeading)
            {
                if (!isOnRightSideCurrentLine)
                {
                    distanceFromCurrentLine *= -1.0;
                }
                abFixHeadingDelta = (steer.heading - abHeading);
            }

            //opposite way so right is left
            else
            {
                if (isOnRightSideCurrentLine)
                {
                    distanceFromCurrentLine *= -1.0;
                }
                abFixHeadingDelta = (steer.heading - abHeading + M_PI);
            }

            //Fix the circular error
            if (abFixHeadingDelta > M_PI) abFixHeadingDelta -= M_PI;
            else if (abFixHeadingDelta < M_PI) abFixHeadingDelta += M_PI;

            if (abFixHeadingDelta > glm::PIBy2) abFixHeadingDelta -= M_PI;
            else if (abFixHeadingDelta < -glm::PIBy2) abFixHeadingDelta += M_PI;

            abFixHeadingDelta *= vehicle.stanleyHeadingErrorGain;
            if (abFixHeadingDelta > 0.74) abFixHeadingDelta = 0.74;
            if (abFixHeadingDelta < -0.74) abFixHeadingDelta = -0.74;

            steerAngleCT = atan((distanceFromCurrentLine * vehicle.stanleyGain) / ((fabs(pn.speed) * 0.277777) + 1));

            if (steerAngleCT > 0.74) steerAngleCT = 0.74;
            if (steerAngleCT < -0.74) steerAngleCT = -0.74;

            if (pn.speed > -0.1)
                steerAngleCT = glm::toDegrees((steerAngleCT + abFixHeadingDelta) * -1.0);
            else
                steerAngleCT = glm::toDegrees((steerAngleCT - abFixHeadingDelta) * -1.0);

            if (steerAngleCT < -vehicle.maxSteerAngle) steerAngleCT = -vehicle.maxSteerAngle;
            if (steerAngleCT > vehicle.maxSteerAngle) steerAngleCT = vehicle.maxSteerAngle;

            //Convert to millimeters
            distanceFromCurrentLine = glm::roundMidAwayFromZero(distanceFromCurrentLine * 1000.0);
        }
        else
        {
            //find the closest 2 points to current fix
            for (int t = 0; t < ptCount; t++)
            {
                double dist = ((pivot.easting - ctList[t].easting) * (pivot.easting - ctList[t].easting))
                                + ((pivot.northing - ctList[t].northing) * (pivot.northing - ctList[t].northing));
                if (dist < minDistA)
                {
                    minDistB = minDistA;
                    B = A;
                    minDistA = dist;
                    A = t;
                }
                else if (dist < minDistB)
                {
                    minDistB = dist;
                    B = t;
                }
            }

            //just need to make sure the points continue ascending in list order or heading switches all over the place
            if (A > B) { C = A; A = B; B = C; }

            //get the distance from currently active AB line
            //x2-x1
            double dx = ctList[B].easting - ctList[A].easting;
            //z2-z1
            double dy = ctList[B].northing - ctList[A].northing;

            if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dy) < glm::DOUBLE_EPSILON) return;

            //abHeading = atan2(dz, dx);
            abHeading = ctList[A].heading;

            //how far from current AB Line is fix
            distanceFromCurrentLine = ((dy * pn.fix.easting) - (dx * pn.fix.northing) + (ctList[B].easting
                        * ctList[A].northing) - (ctList[B].northing * ctList[A].easting))
                            / sqrt((dy * dy) + (dx * dx));

            //are we on the right side or not
            isOnRightSideCurrentLine = distanceFromCurrentLine > 0;

            //absolute the distance
            distanceFromCurrentLine = fabs(distanceFromCurrentLine);

            // ** Pure pursuit ** - calc point on ABLine closest to current position
            double U = (((pivot.easting - ctList[A].easting) * dx) + ((pivot.northing - ctList[A].northing) * dy))
                        / ((dx * dx) + (dy * dy));

            rEastCT = ctList[A].easting + (U * dx);
            rNorthCT = ctList[A].northing + (U * dy);

            ////determine if the point is between 2 points initially determined
            //double minx, maxx, miny, maxy;

            //minx = Math.Min(ctList[A].northing, ctList[B].northing);
            //maxx = Math.Max(ctList[A].northing, ctList[B].northing);

            //miny = Math.Min(ctList[A].easting, ctList[B].easting);
            //maxy = Math.Max(ctList[A].easting, ctList[B].easting);

            //isValid = (rNorthCT >= minx && rNorthCT <= maxx) && (rEastCT >= miny && rEastCT <= maxy);
            //if (!isValid)
            //{
            //    //invalid distance so tell AS module
            //    distanceFromCurrentLine = 32000;
            //    mf.guidanceLineDistanceOff = 32000;
            //    return;
            //}

            //Subtract the two headings, if > 1.57 its going the opposite heading as refAB
            abFixHeadingDelta = (fabs(vehicle.fixHeading - abHeading));
            if (abFixHeadingDelta >= M_PI) abFixHeadingDelta = fabs(abFixHeadingDelta - glm::twoPI);

            //used for accumulating distance to find goal point
            double distSoFar;

            //update base on autosteer settings and distance from line
            double goalPointDistance = vehicle.updateGoalPointDistance(pn, distanceFromCurrentLine);
            //not needed; vehicle.updateGoalPointDistance does this for us.
            //mf.lookaheadActual = goalPointDistance;

            // used for calculating the length squared of next segment.
            double tempDist = 0.0;

            if (abFixHeadingDelta >= glm::PIBy2)
            {
                //counting down
                isABSameAsVehicleHeading = false;
                distSoFar = glm::distance(ctList[A], rEastCT, rNorthCT);
                //Is this segment long enough to contain the full lookahead distance?
                if (distSoFar > goalPointDistance)
                {
                    //treat current segment like an AB Line
                    goalPointCT.easting = rEastCT - (sin(ctList[A].heading) * goalPointDistance);
                    goalPointCT.northing = rNorthCT - (cos(ctList[A].heading) * goalPointDistance);
                }

                //multiple segments required
                else
                {
                    //cycle thru segments and keep adding lengths. check if start and break if so.
                    while (A > 0)
                    {
                        B--; A--;
                        tempDist = glm::distance(ctList[B], ctList[A]);

                        //will we go too far?
                        if ((tempDist + distSoFar) > goalPointDistance)
                        {
                            //A++; B++;
                            break; //tempDist contains the full length of next segment
                        }
                        else
                        {
                            distSoFar += tempDist;
                        }
                    }

                    double t = (goalPointDistance - distSoFar); // the remainder to yet travel
                    t /= tempDist;

                    goalPointCT.easting = (((1 - t) * ctList[B].easting) + (t * ctList[A].easting));
                    goalPointCT.northing = (((1 - t) * ctList[B].northing) + (t * ctList[A].northing));
                }
            }
            else
            {
                //counting up
                isABSameAsVehicleHeading = true;
                distSoFar = glm::distance(ctList[B], rEastCT, rNorthCT);

                //Is this segment long enough to contain the full lookahead distance?
                if (distSoFar > goalPointDistance)
                {
                    //treat current segment like an AB Line
                    goalPointCT.easting = rEastCT + (sin(ctList[A].heading) * goalPointDistance);
                    goalPointCT.northing = rNorthCT + (cos(ctList[A].heading) * goalPointDistance);
                }

                //multiple segments required
                else
                {
                    //cycle thru segments and keep adding lengths. check if end and break if so.
                    // ReSharper disable once LoopVariableIsNeverChangedInsideLoop
                    while (B < ptCount - 1)
                    {
                        B++; A++;
                        tempDist = glm::distance(ctList[B], ctList[A]);

                        //will we go too far?
                        if ((tempDist + distSoFar) > goalPointDistance)
                        {
                            //A--; B--;
                            break; //tempDist contains the full length of next segment
                        }

                        distSoFar += tempDist;
                    }

                    //xt = (((1 - t) * x0 + t * x1)
                    //yt = ((1 - t) * y0 + t * y1))

                    double t = (goalPointDistance - distSoFar); // the remainder to yet travel
                    t /= tempDist;

                    goalPointCT.easting = (((1 - t) * ctList[A].easting) + (t * ctList[B].easting));
                    goalPointCT.northing = (((1 - t) * ctList[A].northing) + (t * ctList[B].northing));
                }
            }

            //calc "D" the distance from pivot axle to lookahead point
            double goalPointDistanceSquared = glm::distanceSquared(goalPointCT.northing, goalPointCT.easting, pivot.northing, pivot.easting);

            //calculate the the delta x in local coordinates and steering angle degrees based on wheelbase
            double localHeading = glm::twoPI - vehicle.fixHeading;
            ppRadiusCT = goalPointDistanceSquared / (2 * (((goalPointCT.easting - pivot.easting) * cos(localHeading)) + ((goalPointCT.northing - pivot.northing) * sin(localHeading))));

            steerAngleCT = glm::toDegrees(atan(2 * (((goalPointCT.easting - pivot.easting) * cos(localHeading))
                + ((goalPointCT.northing - pivot.northing) * sin(localHeading))) * vehicle.wheelbase / goalPointDistanceSquared));

            if (steerAngleCT < -vehicle.maxSteerAngle) steerAngleCT = -vehicle.maxSteerAngle;
            if (steerAngleCT > vehicle.maxSteerAngle) steerAngleCT = vehicle.maxSteerAngle;

            if (ppRadiusCT < -500) ppRadiusCT = -500;
            if (ppRadiusCT > 500) ppRadiusCT = 500;

            radiusPointCT.easting = pivot.easting + (ppRadiusCT * cos(localHeading));
            radiusPointCT.northing = pivot.northing + (ppRadiusCT * sin(localHeading));

            //angular velocity in rads/sec  = 2PI * m/sec * radians/meters
            double angVel = glm::twoPI * 0.277777 * pn.speed * (tan(glm::toRadians(steerAngleCT))) / vehicle.wheelbase;

            //clamp the steering angle to not exceed safe angular velocity
            if (fabs(angVel) > vehicle.maxAngularVelocity)
            {
                steerAngleCT = glm::toDegrees(steerAngleCT > 0 ?
                        (atan((vehicle.wheelbase * vehicle.maxAngularVelocity) / (glm::twoPI * pn.speed * 0.277777)))
                    : (atan((vehicle.wheelbase * -vehicle.maxAngularVelocity) / (glm::twoPI * pn.speed * 0.277777))));
            }
            //Convert to centimeters
            distanceFromCurrentLine = glm::roundMidAwayFromZero(distanceFromCurrentLine * 1000.0);

            //distance is negative if on left, positive if on right
            //if you're going the opposite direction left is right and right is left
            //double temp;
            if (isABSameAsVehicleHeading)
            {
                if (!isOnRightSideCurrentLine) distanceFromCurrentLine *= -1.0;
            }

            //opposite way so right is left
            else if (isOnRightSideCurrentLine)
            {
                distanceFromCurrentLine *= -1.0;
            }
        }

        //fill in the autosteer variables
        vehicle.guidanceLineDistanceOff = vehicle.distanceDisplay = (int)distanceFromCurrentLine;
        vehicle.guidanceLineSteerAngle = (int)(steerAngleCT * 100);
    }
    else
    {
        //invalid distance so tell AS module
        distanceFromCurrentLine = 32000;
        vehicle.guidanceLineDistanceOff = 32000;
    }
}

void CContour::calculateContourHeadings()
{
    //to calc heading based on next and previous points to give an average heading.
    int cnt = ctList.size();
    QVector<Vec3> arr = ctList;
    cnt--;
    ctList.clear();

    //middle points
    for (int i = 1; i < cnt; i++)
    {
        Vec3 pt3 = arr[i];
        pt3.heading = atan2(arr[i + 1].easting - arr[i - 1].easting, arr[i + 1].northing - arr[i - 1].northing);
        if (pt3.heading < 0) pt3.heading += glm::twoPI;
        ctList.append(pt3);
    }
}

//draw the red follow me line
void CContour::drawContourLine(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, CVehicle &vehicle)
{
    USE_SETTINGS;
    ////draw the guidance line
    int ptCount = ctList.size();
    if (ptCount < 2) return;


    gl->glLineWidth(SETTINGS_DISPLAY_LINEWIDTH);

    GLHelperOneColor gldraw;

    for (int h = 0; h < ptCount; h++)
        gldraw.append(QVector3D(ctList[h].easting, ctList[h].northing, 0));

    gldraw.draw(gl,mvp, QColor::fromRgbF(0.98f, 0.2f, 0.980f),
                GL_LINE_STRIP, SETTINGS_DISPLAY_LINEWIDTH);

    gldraw.clear();

    for (int h = 0; h < ptCount; h++)
        gldraw.append(QVector3D(ctList[h].easting, ctList[h].northing, 0));

    gldraw.draw(gl,mvp,QColor::fromRgbF(0.87f, 08.7f, 0.25f),
                GL_POINTS, SETTINGS_DISPLAY_LINEWIDTH);

    if (SETTINGS_DISPLAY_ISPUREON && distanceFromCurrentLine != 32000 && !vehicle.isStanleyUsed)
    {
        gldraw.clear();
        gldraw.append(QVector3D(goalPointCT.easting, goalPointCT.northing, 0.0));
        gldraw.draw(gl, mvp, QColor::fromRgbF(1.0f, 0.95f, 0.095f),
                    GL_POINTS, 6.0f);
    }
}

//Reset the contour to zip
void CContour::resetContour()
{
    stripList.clear();

    if (!ptList.isNull()) ptList->clear();
    ctList.clear();
    ctListBufferCurrent = false;
}
