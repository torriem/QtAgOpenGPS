#include "ctram.h"
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <math.h>
#include "glutils.h"
#include "cboundary.h"
#include "ccamera.h"
#include "aogproperty.h"
#include "glm.h"

//TODO: move all these to own file, centralize the names we're using
//      to refer to settings
#define DEFAULT_EQWIDTH 24

CTram::CTram()
{
    loadSettings();
    IsTramOuterOrInner();
    displayMode=0;
}

void CTram::loadSettings()
{
    tramWidth = property_setTram_tramWidth;
    halfWheelTrack = (double)property_setVehicle_trackWidth * 0.5;
    passes = property_setTram_passes;
}

void CTram::IsTramOuterOrInner()
{
    isOuter = ((int)(tramWidth / (double)property_setVehicle_toolWidth + 0.5)) % 2 == 0;
    if ((bool)property_setTool_isTramOuterInverted) isOuter = !isOuter;
}

void CTram::DrawTram(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, CCamera &camera)
{
    double lineWidth;

    GLHelperOneColor gldraw;
    QColor color;

    if (camera.camSetDistance > -250) lineWidth = 4.0f;
    else lineWidth=2.0f;
    color.setRgbF(0.30f, 0.93692f, 0.7520f, 0.3);

    if (displayMode == 1 || displayMode == 2)
    {
        if (tramList.count() > 0)
        {
            for (int i = 0; i < tramList.count(); i++)
            {
                gldraw.clear();
                for (int h = 0; h < (*tramList[i]).count(); h++)
                    gldraw.append(QVector3D((*tramList[i])[h].easting, (*tramList[i])[h].northing, 0));
                gldraw.draw(gl,mvp,color,GL_LINE_STRIP,lineWidth);
            }
        }
    }

    if (displayMode == 1 || displayMode == 3)
    {
        if (tramBndOuterArr.count() > 0)
        {
            gldraw.clear();
            for (int h = 0; h < tramBndOuterArr.count(); h++)
                gldraw.append(QVector3D(tramBndOuterArr[h].easting, tramBndOuterArr[h].northing, 0));
            gldraw.draw(gl,mvp,color,GL_LINE_STRIP,lineWidth);
            gldraw.clear();
            for (int h = 0; h < tramBndInnerArr.count(); h++)
                gldraw.append(QVector3D(tramBndInnerArr[h].easting, tramBndInnerArr[h].northing, 0));
            gldraw.draw(gl,mvp,color,GL_LINE_STRIP,lineWidth);
        }
    }
}

void CTram::BuildTramBnd(const CBoundary &bnd)
{
    bool isBndExist = bnd.bndList.count() != 0;

    if (isBndExist)
    {
        CreateBndOuterTramTrack(bnd);
        CreateBndInnerTramTrack(bnd);
    }
    else
    {
        tramBndOuterArr.clear();
        tramBndInnerArr.clear();
    }
}

void CTram::CreateBndInnerTramTrack(const CBoundary &bnd)
{
    double tramWidth = property_setTram_tramWidth;
    double halfWheelTrack = (double)property_setVehicle_trackWidth * 0.5;

    int ptCount = bnd.bndList[0].fenceLine.count();
    tramBndInnerArr.clear();

    //outside point
    Vec2 pt3;

    double distSq = ((tramWidth * 0.5) + halfWheelTrack) * ((tramWidth * 0.5) + halfWheelTrack) * 0.999;

    //make the boundary tram outer array
    for (int i = 0; i < ptCount; i++)
    {
        //calculate the point inside the boundary
        pt3.easting = bnd.bndList[0].fenceLine[i].easting -
            (sin(glm::PIBy2 + bnd.bndList[0].fenceLine[i].heading) * (tramWidth * 0.5 + halfWheelTrack));

        pt3.northing = bnd.bndList[0].fenceLine[i].northing -
            (cos(glm::PIBy2 + bnd.bndList[0].fenceLine[i].heading) * (tramWidth * 0.5 + halfWheelTrack));

        bool Add = true;

        for (int j = 0; j < ptCount; j++)
        {
            double check = glm::DistanceSquared(pt3.northing, pt3.easting,
                                bnd.bndList[0].fenceLine[j].northing, bnd.bndList[0].fenceLine[j].easting);
            if (check < distSq)
            {
                Add = false;
                break;
            }
        }

        if (Add)
        {
            if (tramBndInnerArr.count() > 0)
            {
                double dist = ((pt3.easting - tramBndInnerArr[tramBndInnerArr.count() - 1].easting) * (pt3.easting - tramBndInnerArr[tramBndInnerArr.count() - 1].easting))
                              + ((pt3.northing - tramBndInnerArr[tramBndInnerArr.count() - 1].northing) * (pt3.northing - tramBndInnerArr[tramBndInnerArr.count() - 1].northing));
                if (dist > 2)
                    tramBndInnerArr.append(pt3);
            }
            else tramBndInnerArr.append(pt3);
        }
    }
}

void CTram::CreateBndOuterTramTrack(const CBoundary &bnd)
{
    double tramWidth = property_setTram_tramWidth;
    double halfWheelTrack = (double)property_setVehicle_trackWidth * 0.5;

    //count the points from the boundary
    int ptCount = bnd.bndList[0].fenceLine.count();
    tramBndOuterArr.clear();

    //outside point
    Vec2 pt3;

    double distSq = ((tramWidth * 0.5) - halfWheelTrack) * ((tramWidth * 0.5) - halfWheelTrack) * 0.999;

    //make the boundary tram outer array
    for (int i = 0; i < ptCount; i++)
    {
        //calculate the point inside the boundary
        pt3.easting = bnd.bndList[0].fenceLine[i].easting -
                      (sin(glm::PIBy2 + bnd.bndList[0].fenceLine[i].heading) * (tramWidth * 0.5 - halfWheelTrack));

        pt3.northing = bnd.bndList[0].fenceLine[i].northing -
                       (cos(glm::PIBy2 + bnd.bndList[0].fenceLine[i].heading) * (tramWidth * 0.5 - halfWheelTrack));

        bool Add = true;

        for (int j = 0; j < ptCount; j++)
        {
            double check = glm::DistanceSquared(pt3.northing, pt3.easting,
                                               bnd.bndList[0].fenceLine[j].northing, bnd.bndList[0].fenceLine[j].easting);
            if (check < distSq)
            {
                Add = false;
                break;
            }
        }

        if (Add)
        {
            if (tramBndOuterArr.count() > 0)
            {
                double dist = ((pt3.easting - tramBndOuterArr[tramBndOuterArr.count() - 1].easting) * (pt3.easting - tramBndOuterArr[tramBndOuterArr.count() - 1].easting))
                              + ((pt3.northing - tramBndOuterArr[tramBndOuterArr.count() - 1].northing) * (pt3.northing - tramBndOuterArr[tramBndOuterArr.count() - 1].northing));
                if (dist > 2)
                    tramBndOuterArr.append(pt3);
            }
            else tramBndOuterArr.append(pt3);
        }
    }
}

