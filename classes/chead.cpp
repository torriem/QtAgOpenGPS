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
#include "aogsettings.h"

CHead::CHead(QObject *parent) : QObject(parent)
{
    singleSpaceHeadlandDistance = 18;
    isOn = false;

    headArr.append(CHeadLines());
    isToolUp = true;
}

void CHead::setHydPosition(double currentSpeed)
{
    USE_SETTINGS;

    if (SETTINGS_VEHICLE_ISHYDLIFTON && currentSpeed > 0.2 ) //TODO: && mf.autoBtnState == FormGPS.btnStates.Auto
    {
        if (isToolInHeadland)
        {
            emit moveHydraulics(2);
            isToolUp = true;
        }
        else
        {
            emit moveHydraulics(1);
            isToolUp = false;
        }
    }
}

void CHead::whereAreToolCorners(CTool &tool)
{
    if (headArr[0].hdLine.count() == 0)
    {
        return;
    }
    else
    {
        bool isLeftInWk, isRightInWk = true;

        if (isOn)
        {
            for (int j = 0; j < tool.numOfSections; j++)
            {
                if (j == 0)
                {
                    //only one first left point, the rest are all rights moved over to left
                    isLeftInWk = headArr[0].isPointInHeadArea(tool.section[j].leftPoint);
                    isRightInWk = headArr[0].isPointInHeadArea(tool.section[j].rightPoint);

                    //save left side
                    tool.isLeftSideInHeadland = !isLeftInWk;

                    //merge the two sides into in or out
                    tool.section[j].isInHeadlandArea = !isLeftInWk && !isRightInWk;

                }
                else
                {
                    //grab the right of previous section, its the left of this section
                    isLeftInWk = isRightInWk;
                    isRightInWk = headArr[0].isPointInHeadArea(tool.section[j].rightPoint);

                    tool.section[j].isInHeadlandArea = !isLeftInWk && !isRightInWk;
                }
            }

            //save right side
            tool.isRightSideInHeadland = !isRightInWk;

            //is the tool in or out based on endpoints
            isToolOuterPointsInHeadland = tool.isLeftSideInHeadland && tool.isRightSideInHeadland;
        }
        else
        {
            //set all to true;
            isToolOuterPointsInHeadland = true;
        }
    }
}

void CHead::whereAreToolLookOnPoints(const CVehicle &vehicle, CTool &tool)
{
    if (headArr[0].hdLine.count() == 0)
    {
        return;
    }
    else
    {
        Vec3 toolFix = vehicle.toolPos;
        double sinAB = sin(toolFix.heading);
        double cosAB = cos(toolFix.heading);

        double pos = tool.section[0].rpSectionWidth;
        double mOn = (tool.lookAheadDistanceOnPixelsRight - tool.lookAheadDistanceOnPixelsLeft) / tool.rpWidth;
        double endHeight = (tool.lookAheadDistanceOnPixelsLeft + (mOn * pos))*0.1;

        for (int j = 0; j < tool.numOfSections; j++)
        {
            if (j == 0)
            {
                downL.easting = tool.section[j].leftPoint.easting + (sinAB * tool.lookAheadDistanceOnPixelsLeft * 0.1);
                downL.northing = tool.section[j].leftPoint.northing + (cosAB * tool.lookAheadDistanceOnPixelsLeft * 0.1);

                downR.easting = tool.section[j].rightPoint.easting + (sinAB * endHeight);
                downR.northing = tool.section[j].rightPoint.northing + (cosAB * endHeight);

                isLookLeftIn = isPointInsideHeadLine(downL);
                isLookRightIn = isPointInsideHeadLine(downR);

                tool.section[j].isLookOnInHeadland = !isLookLeftIn && !isLookRightIn;
                isLookLeftIn = isLookRightIn;
            }
            else
            {
                pos += tool.section[j].rpSectionWidth;
                endHeight = (tool.lookAheadDistanceOnPixelsLeft + (mOn * pos))*0.1;

                downR.easting = tool.section[j].rightPoint.easting + (sinAB * endHeight);
                downR.northing = tool.section[j].rightPoint.northing + (cosAB * endHeight);

                isLookRightIn = isPointInsideHeadLine(downR);
                tool.section[j].isLookOnInHeadland = !isLookLeftIn && !isLookRightIn;
                isLookLeftIn = isLookRightIn;
            }
        }
    }

}

void CHead::drawHeadLines(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, int lineWidth)
{
    if (headArr[0].hdLine.count() > 0 && isOn) headArr[0].drawHeadLine(gl, mvp, lineWidth);
}

void CHead::drawHeadLinesBack(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
    if (headArr[0].hdLine.count() > 0 && isOn) headArr[0].drawHeadLineBackBuffer(gl, mvp);
}

bool CHead::isPointInsideHeadLine(Vec2 pt)
{
    //if inside outer boundary, then potentially add
    if (headArr.count() > 0 && headArr[0].isPointInHeadArea(pt))
    {
        return true;
    }
    else
    {
        return false;
    }
}
