#include "cboundary.h"
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include "vec2.h"
#include "vec3.h"
#include "glm.h"
#include "cvehicle.h"
#include "ctool.h"
#include "cpgn.h"

void CBoundary::SetHydPosition(btnStates autoBtnState, CPGN_EF &p_239, CVehicle &vehicle)
{
    if (vehicle.isHydLiftOn && vehicle.avgSpeed > 0.2 && autoBtnState == btnStates::Auto)
    {
        if (isToolInHeadland)
        {
            p_239.pgn[p_239.hydLift] = 2;
            //TODO: implement sounds
            emit soundHydLiftChange(isToolInHeadland);
            /*
            if (mf.sounds.isHydLiftChange != isToolInHeadland)
            {
                if (mf.sounds.isHydLiftSoundOn) mf.sounds.sndHydLiftUp.Play();
                mf.sounds.isHydLiftChange = isToolInHeadland;
            }
            */
        }
        else
        {
            p_239.pgn[p_239.hydLift] = 1;
            //TODO: implement sounds
            emit soundHydLiftChange(isToolInHeadland);
            /*
            if (mf.sounds.isHydLiftChange != isToolInHeadland)
            {
                if (mf.sounds.isHydLiftSoundOn) mf.sounds.sndHydLiftDn.Play();
                mf.sounds.isHydLiftChange = isToolInHeadland;
            }
            */
        }
    }
}

void CBoundary::WhereAreToolCorners(CTool &tool)
{
    if (bndList.count() > 0 && bndList[0].hdLine.count() > 0)
    {
        bool isLeftInWk, isRightInWk = true;

        for (int j = 0; j < tool.numOfSections; j++)
        {
            isLeftInWk = j == 0 ? IsPointInsideHeadArea(tool.section[j].leftPoint) : isRightInWk;
            isRightInWk = IsPointInsideHeadArea(tool.section[j].rightPoint);

            //save left side
            if (j == 0)
                tool.isLeftSideInHeadland = !isLeftInWk;

            //merge the two sides into in or out
            tool.section[j].isInHeadlandArea = !isLeftInWk && !isRightInWk;
        }

        //save right side
        tool.isRightSideInHeadland = !isRightInWk;

        //is the tool in or out based on endpoints
        isToolOuterPointsInHeadland = tool.isLeftSideInHeadland && tool.isRightSideInHeadland;
    }
}

void CBoundary::WhereAreToolLookOnPoints(CVehicle &vehicle, CTool &tool)
{
    if (bndList.count() > 0 && bndList[0].hdLine.count() > 0)
    {
        bool isLookRightIn = false;

        Vec3 toolFix = vehicle.toolPos;
        double sinAB = sin(toolFix.heading);
        double cosAB = cos(toolFix.heading);

        //generated box for finding closest point
        double pos = 0;
        double mOn = (tool.lookAheadDistanceOnPixelsRight - tool.lookAheadDistanceOnPixelsLeft) / tool.rpWidth;

        for (int j = 0; j < tool.numOfSections; j++)
        {
            bool isLookLeftIn = j == 0 ? IsPointInsideHeadArea(Vec2(
                                    tool.section[j].leftPoint.easting + (sinAB * tool.lookAheadDistanceOnPixelsLeft * 0.1),
                                    tool.section[j].leftPoint.northing + (cosAB * tool.lookAheadDistanceOnPixelsLeft * 0.1))) : isLookRightIn;

            pos += tool.section[j].rpSectionWidth;
            double endHeight = (tool.lookAheadDistanceOnPixelsLeft + (mOn * pos)) * 0.1;

            isLookRightIn = IsPointInsideHeadArea(Vec2(
                tool.section[j].rightPoint.easting + (sinAB * endHeight),
                tool.section[j].rightPoint.northing + (cosAB * endHeight)));

            tool.section[j].isLookOnInHeadland = !isLookLeftIn && !isLookRightIn;
        }
    }
}

bool CBoundary::IsPointInsideHeadArea(Vec2 pt)
{
    //if inside outer boundary, then potentially add
    if (glm::IsPointInPolygon(bndList[0].hdLine, pt))
    {
        for (int i = 1; i < bndList.count(); i++)
        {
            if (glm::IsPointInPolygon(bndList[i].hdLine,pt))
            {
                //point is in an inner turn area but inside outer
                return false;
            }
        }
        return true;
    }
    return false;
}
