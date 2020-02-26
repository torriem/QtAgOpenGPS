#include "ctram.h"
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <math.h>
#include "glutils.h"
#include "ctool.h"
#include "cboundary.h"
#include "aogsettings.h"
#include "glm.h"

//TODO: move all these to own file, centralize the names we're using
//      to refer to settings
#define DEFAULT_EQWIDTH 24

CTram::CTram()
{
    USE_SETTINGS;
    tramWidth = SETTINGS_TRAM_EQWIDTH;
    wheelTrack = SETTINGS_TRAM_WHEELSPACING;
    halfWheelTrack = wheelTrack * 0.5;

    passes = SETTINGS_TRAM_PASSES;
    //abOffset = (tool->toolWidth - tool->toolOverlap) / 2.0;
    abOffset = 0;
    displayMode = 0;
}

void CTram::drawTramBnd(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
    if(tramBndArr.size() > 0) {
        QOpenGLBuffer tramBuffer;
        QVector<QVector3D> vertices;
        for (int h = 0; h < tramBndArr.size(); h++)
            vertices.append(QVector3D(tramBndArr[h].easting, tramBndArr[h].northing, 0));

        tramBuffer.create();
        tramBuffer.bind();
        tramBuffer.allocate(vertices.data(), tramBndArr.size() * sizeof(QVector3D));
        tramBuffer.release();

        glDrawArraysColor(gl, mvp, GL_TRIANGLE_STRIP,
                          QColor::fromRgbF(0.8630f, 0.73692f, 0.60f, 0.25),
                          tramBuffer,GL_SHORT,tramBndArr.size(),1.0f);
    }
}

void CTram::buildTramBnd(const CBoundary &bnd)
{
    bool isBndExist = bnd.bndArr.size() != 0;

    if (isBndExist)
    {
        createBndTramRef(bnd);
        createOuterTram();
        preCalcTurnLines();
    }
    else
    {
        outArr.clear();
        tramBndArr.clear();
    }
}

void CTram::createBndTramRef(const CBoundary &bnd)
{
    //count the points from the boundary
    int ptCount = bnd.bndArr[0].bndLine.size();
    outArr.clear();

    //outside point
    Vec3 pt3;

    double distSq = ((tramWidth * 0.5) - halfWheelTrack) * ((tramWidth * 0.5) - halfWheelTrack) * 0.97;
    bool fail = false;

    //make the boundary tram outer array
    for (int i = 0; i < ptCount; i++)
    {
        //calculate the point inside the boundary
        pt3.easting = bnd.bndArr[0].bndLine[i].easting -
            (sin(glm::PIBy2 + bnd.bndArr[0].bndLine[i].heading) * (tramWidth * 0.5 - halfWheelTrack));

        pt3.northing = bnd.bndArr[0].bndLine[i].northing -
            (cos(glm::PIBy2 + bnd.bndArr[0].bndLine[i].heading) * (tramWidth * 0.5 - halfWheelTrack));

        for (int j = 0; j < ptCount; j++)
        {
            double check = glm::distanceSquared(pt3.northing, pt3.easting,
                                bnd.bndArr[0].bndLine[j].northing, bnd.bndArr[0].bndLine[j].easting);
            if (check < distSq)
            {
                fail = true;
                break;
            }
        }

        if (!fail)
        {
            pt3.heading = bnd.bndArr[0].bndLine[i].heading;
            outArr.append(pt3);
        }
        fail = false;
    }

    int cnt = outArr.size();
    if (cnt < 6) return;

    const double spacing = 2.0;
    double distance;
    for (int i = 0; i < cnt - 1; i++)
    {
        distance = glm::distance(outArr[i], outArr[i + 1]);
        if (distance < spacing)
        {
            outArr.removeAt(i + 1);
            cnt = outArr.size();
            i--;
        }
    }

}

void CTram::createOuterTram()
{
    //build the outer boundary
    tramBndArr.clear();

    int cnt = outArr.size();

    if (cnt > 0)
    {
        Vec2 pt;
        Vec2 pt2;

        for (int i = 0; i < cnt; i++)
        {
            pt.easting = outArr[i].easting;
            pt.northing = outArr[i].northing;
            tramBndArr.append(pt);

            pt2.easting = outArr[i].easting -
                (sin(glm::PIBy2 + outArr[i].heading) * wheelTrack);

            pt2.northing = outArr[i].northing -
                (cos(glm::PIBy2 + outArr[i].heading) * wheelTrack);
            tramBndArr.append(pt2);
        }
    }

}

bool CTram::isPointInTramBndArea(Vec2 testPointv2)
{
    if (calcList.size() < 3) return false;
    int j = outArr.size() - 1;
    bool oddNodes = false;

    //test against the constant and multiples list the test point
    for (int i = 0; i < outArr.size(); j = i++)
    {
        if ((outArr[i].northing < testPointv2.northing && outArr[j].northing >= testPointv2.northing)
        ||  (outArr[j].northing < testPointv2.northing && outArr[i].northing >= testPointv2.northing))
        {
            oddNodes ^= ((testPointv2.northing * calcList[i].northing) + calcList[i].easting < testPointv2.easting);
        }
    }
    return oddNodes; //true means inside.

}

void CTram::preCalcTurnLines()
{
    int j = outArr.size() - 1;
    //clear the list, constant is easting, multiple is northing
    calcList.clear();
    Vec2 constantMultiple(0, 0);

    for (int i = 0; i < outArr.size(); j = i++)
    {
        //check for divide by zero
        if (fabs(outArr[i].northing - outArr[j].northing) < glm::DOUBLE_EPSILON)
        {
            constantMultiple.easting = outArr[i].easting;
            constantMultiple.northing = 0;
            calcList.append(constantMultiple);
        }
        else
        {
            //determine constant and multiple and add to list
            constantMultiple.easting = outArr[i].easting - ((outArr[i].northing * outArr[j].easting)
                            / (outArr[j].northing - outArr[i].northing)) + ((outArr[i].northing * outArr[i].easting)
                                / (outArr[j].northing - outArr[i].northing));
            constantMultiple.northing = (outArr[j].easting - outArr[i].easting) / (outArr[j].northing - outArr[i].northing);
            calcList.append(constantMultiple);
        }
    }
}
