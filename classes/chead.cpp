#include "chead.h"
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include "vec2.h"
#include "vec3.h"
#include "glm.h"
#include "glutils.h"
#include "cvehicle.h"
#include "ctool.h"
#include "cheadlines.h"
#include "common.h"

CHead::CHead(QObject *parent) : QObject(parent)
{
    singleSpaceHeadlandDistance = 18;
    isOn = false;
    leftToolDistance = 99999;
    rightToolDistance = 99999;
    isToolUp = true;
    headArr.append(CHeadLines());
}

bool CHead::findHeadlandDistance(const CVehicle &vehicle, const CTool &tool)
{
    if (headArr[0].hdLine.size() == 0)
    {
        leftToolDistance = 99999;
        rightToolDistance = 99999;
        return false;
    }
    else
    {

        leftToolDistance = 999999;
        rightToolDistance = 999999;

        double minDistA = 1000000, minDistB = 1000000;

        int ptCount = headArr[0].hdLine.size();

        //find the closest 2 points to current fix
        for (int t = 0; t < ptCount; t++)
        {
            double dist = glm::distanceSquared(tool.section[0].leftPoint, headArr[0].hdLine[t]);
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

        //just need to make sure the points continue ascending or heading switches all over the place
        if (A > B) { Q = A; A = B; B = Q; }

        double dx = headArr[0].hdLine[B].easting - headArr[0].hdLine[A].easting;
        double dz = headArr[0].hdLine[B].northing - headArr[0].hdLine[A].northing;

        if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dz) < glm::DOUBLE_EPSILON) return false;

        //abHeading = Math.Atan2(dz, dx);
        //double abHeading = curList[A].heading;

        //how far from current AB Line is fix
        leftToolDistance = ((dz * tool.section[0].leftPoint.easting) - (dx * tool.section[0].leftPoint.northing) + (headArr[0].hdLine[B].easting
                    * headArr[0].hdLine[A].northing) - (headArr[0].hdLine[B].northing * headArr[0].hdLine[A].easting))
                        / sqrt((dz * dz) + (dx * dx));

        //are we on the right side or not
        //bool isOnRightSideCurrentLine = leftToolDistance > 0;
        //absolute the distance
        //leftToolDistance = fabs(leftToolDistance);

        //find the closest 2 points to current fixm for right side
        minDistA = 1000000;
        minDistB = 1000000;

        for (int t = 0; t < ptCount; t++)
        {
            double dist = glm::distanceSquared(tool.section[tool.numOfSections-1].rightPoint, headArr[0].hdLine[t]);
            if (dist < minDistA)
            {
                minDistB = minDistA;
                D = C;
                minDistA = dist;
                C = t;
            }
            else if (dist < minDistB)
            {
                minDistB = dist;
                D = t;
            }
        }

        //just need to make sure the points continue ascending or heading switches all over the place
        if (C > D) { Q = C; C = D; D = Q; }

        dx = headArr[0].hdLine[D].easting - headArr[0].hdLine[C].easting;
        dz = headArr[0].hdLine[D].northing - headArr[0].hdLine[C].northing;

        if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dz) < glm::DOUBLE_EPSILON) return false;

        //abHeading = Math.Atan2(dz, dx);
        //double abHeading = curList[C].heading;

        //how far from current AB Line is fix
        rightToolDistance = ((dz * tool.section[tool.numOfSections - 1].rightPoint.easting) - (dx * tool.section[tool.numOfSections - 1].rightPoint.northing) + (headArr[0].hdLine[D].easting
                    * headArr[0].hdLine[C].northing) - (headArr[0].hdLine[D].northing * headArr[0].hdLine[C].easting))
                        / sqrt((dz * dz) + (dx * dx));

        //are we on the right side or not
        //bool isOnRightSideCurrentLine = leftToolDistance > 0;

        //absolute the distance
        //leftToolDistance = fabs(leftToolDistance);
        //double scanWidthR = (tool.toolWidth * 0.75);

        if (leftToolDistance > 0 && rightToolDistance > 0)
        {
            isToolInWorkArea = true;
            isToolInBothPlaces = isToolInHeadland = false;
        }
        else if (leftToolDistance < 0 && rightToolDistance < 0)
        {
            isToolInHeadland  = true;
            isToolInBothPlaces = isToolInWorkArea = false;
        }
        else
        {
            isToolInBothPlaces = true;
            isToolInHeadland = isToolInWorkArea = false;
        }

        Vec3 toolFix = vehicle.toolPos;
        double headAB = toolFix.heading;

        downL.easting = tool.section[0].leftPoint.easting + (sin(headAB) * vehicle.hydLiftLookAhead * tool.toolFarLeftSpeed);
        downL.northing = tool.section[0].leftPoint.northing + (cos(headAB) * vehicle.hydLiftLookAhead * tool.toolFarLeftSpeed);

        downR.easting = tool.section[tool.numOfSections - 1].rightPoint.easting + (sin(headAB) * vehicle.hydLiftLookAhead * tool.toolFarRightSpeed);
        downR.northing = tool.section[tool.numOfSections - 1].rightPoint.northing + (cos(headAB) * vehicle.hydLiftLookAhead * tool.toolFarRightSpeed);

        //bool isToolFullyOut =
        bool isLookRightIn = isPointInsideHeadLine(downR);
        bool isLookLeftIn = isPointInsideHeadLine(downL);


        if (isLookLeftIn && isLookRightIn)
        {
            isLookInWorkArea = true;
            isLookInBothPlaces = isLookInHealand = false;
        }
        else if (!isLookLeftIn && !isLookRightIn)
        {
            isLookInHealand = true;
            isLookInBothPlaces = isLookInWorkArea = false;
        }
        else
        {
            isLookInBothPlaces = true;
            isLookInHealand = isLookInWorkArea = false;
        }

        //isToolUp = false;

        if (!isLookInHealand && isToolUp) isToolUp = false;
        if (isLookInHealand && isToolInHeadland && !isToolUp) isToolUp = true;

        if (isToolUp) emit moveHydraulics(hydStates::up);
        else emit moveHydraulics(hydStates::down);
        //if (isToolUp) mf.mc.pgn[mf.mc.azRelayData][mf.mc.rdHydLift] = 1;
        //else mf.mc.pgn[mf.mc.azRelayData][mf.mc.rdHydLift] = 0;

        return isToolUp;
    }
}
