#include "cboundary.h"
#include "glm.h"
#include "glutils.h"
#include "aogproperty.h"
#include "cvehicle.h"
#include "cmodulecomm.h"

/*************/
/* CFence.cs */
/*************/
bool CBoundary::IsPointInsideFenceArea(Vec3 testPoint) const
{
    //first where are we, must be inside outer and outside of inner geofence non drive thru turn borders
    if (glm::IsPointInPolygon(bndList[0].fenceLineEar,testPoint))
    {
        for (int i = 1; i < bndList.count(); i++)
        {
            //make sure not inside a non drivethru boundary
            if (bndList[i].isDriveThru) continue;
            if (glm::IsPointInPolygon(bndList[i].fenceLineEar,testPoint))
            {
                return false;
            }
        }
        //we are safely inside outer, outside inner boundaries
        return true;
    }
    return false;
}


bool CBoundary::IsPointInsideFenceArea(Vec2 testPoint) const
{
    //first where are we, must be inside outer and outside of inner geofence non drive thru turn borders
    if (glm::IsPointInPolygon(bndList[0].fenceLineEar,testPoint))
    {
        for (int i = 1; i < bndList.count(); i++)
        {
            //make sure not inside a non drivethru boundary
            if (bndList[i].isDriveThru) continue;
            if (glm::IsPointInPolygon(bndList[i].fenceLineEar,testPoint))
            {
                return false;
            }
        }
        //we are safely inside outer, outside inner boundaries
        return true;
    }
    return false;

}

void CBoundary::DrawFenceLines(const CVehicle &v, const CModuleComm &mc,
                               QOpenGLFunctions *gl,
                                 const QMatrix4x4 &mvp)
{
    QColor color;
    float line_width;
    Vec3 pivot = v.pivotAxlePos;

    GLHelperOneColor gldraw;

    if (!mc.isOutOfBounds)
    {
        color.setRgbF(0.95f, 0.75f, 0.50f);
        line_width = property_setDisplay_lineWidth;
    }
    else
    {
        line_width = (float) property_setDisplay_lineWidth * 3;
        color.setRgbF(0.95f, 0.25f, 0.25f);
    }

    for (int i = 0; i < bndList.count(); i++)
    {
        gldraw.clear();
        for (int j = 0; j < bndList[i].fenceLineEar.count(); j++) {
            gldraw.append(QVector3D(bndList[i].fenceLineEar[j].easting,
                                    bndList[i].fenceLineEar[j].northing, 0));
        }
        gldraw.draw(gl, mvp, color, GL_TRIANGLE_STRIP, line_width);
    }


    //closest points  TooDoo
    gldraw.append(QVector3D(closestTurnPt.easting, closestTurnPt.northing, 0));
    gldraw.draw(gl,mvp,QColor::fromRgbF(0.70f, 0.95f, 0.95f),
                GL_POINTS,6.0f);

    if (bndBeingMadePts.count() > 0)
    {
        GLHelperColors gldraw1;
        ColorVertex cv;


        //the boundary so far
        //vec3 pivot = mf.pivotAxlePos;
        cv.color = QVector4D(0.825f, 0.22f, 0.90f, 1.0f);

        for (int h = 0; h < bndBeingMadePts.count(); h++) {
            cv.vertex = QVector3D(bndBeingMadePts[h].easting, bndBeingMadePts[h].northing, 0);
            gldraw1.append(cv);
        }

        cv.color = QVector4D(0.295f, 0.972f, 0.290f, 1.0f);
        cv.vertex = QVector3D(bndBeingMadePts[0].easting, bndBeingMadePts[0].northing, 0);
        gldraw1.append(cv);

        gldraw1.draw(gl, mvp, GL_LINE_STRIP, property_setDisplay_lineWidth);

        //line from last point to pivot marker
        gldraw.clear();
        if (isDrawRightSide)
        {
            gldraw.append(QVector3D(bndBeingMadePts[0].easting, bndBeingMadePts[0].northing, 0));
            gldraw.append(QVector3D(pivot.easting + (sin(pivot.heading - glm::PIBy2) * -createBndOffset),
                                     pivot.northing + (cos(pivot.heading - glm::PIBy2) * -createBndOffset), 0));

            gldraw.append(QVector3D(bndBeingMadePts[bndBeingMadePts.size() - 1].easting,
                                     bndBeingMadePts[bndBeingMadePts.size() - 1].northing, 0));
        }
        else
        {
            gldraw.append(QVector3D(bndBeingMadePts[0].easting, bndBeingMadePts[0].northing, 0));
            gldraw.append(QVector3D(pivot.easting + (sin(pivot.heading - glm::PIBy2) * createBndOffset),
                                     pivot.northing + (cos(pivot.heading - glm::PIBy2) * createBndOffset), 0));
            gldraw.append(QVector3D(bndBeingMadePts[bndBeingMadePts.size() - 1].easting, bndBeingMadePts[bndBeingMadePts.size() - 1].northing, 0));
        }
        gldraw.draw(gl,mvp,QColor::fromRgbF(0.825f, 0.842f, 0.0f),
                     GL_LINE_STIPPLE,1.0f);

        //boundary points
        gldraw.clear();
        for (int h = 0; h < bndBeingMadePts.size(); h++)
            gldraw.append(QVector3D(bndBeingMadePts[h].easting, bndBeingMadePts[h].northing, 0));

        gldraw.draw(gl,mvp,QColor::fromRgbF(0.0f, 0.95f, 0.95f),
                    GL_POINTS, 6.0f);
    }
}


