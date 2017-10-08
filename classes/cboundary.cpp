#include "cboundary.h"
#include "cnmea.h"
#include "formgps.h"
#include "math.h"
#include <QLocale>
#include <QOpenGLContext>
#include <QOpenGLFunctions_1_1>

CBoundary::CBoundary(FormGPS *mf)
    :mf(mf)
{

}

void CBoundary::resetBoundary()
{
    calcList.clear();
    ptList.clear();
    area = 0;
    areaAcre = "";
    areaHectare = "";

    isDrawRightSide = true;
    isSet = false;
    isOkToAddPoints = false;
}

void CBoundary::preCalcBoundaryLines()
{
    int j = ptList.size() - 1;
    //clear the list, constant is easting, multiple is northing
    calcList.clear();
    Vec2 constantMultiple(0, 0);

    for (int i = 0; i < ptList.size(); j = i++)
    {
        //check for divide by zero
        if (fabs(ptList[i].northing - ptList[j].northing) < 0.00000000001)
        {
            constantMultiple.easting = ptList[i].easting;
            constantMultiple.northing = 0;
        }
        else
        {
            //determine constant and multiple and add to list
            constantMultiple.easting = ptList[i].easting - (ptList[i].northing * ptList[j].easting) /
                            (ptList[j].northing - ptList[i].northing) + (ptList[i].northing * ptList[i].easting) /
                                (ptList[j].northing - ptList[i].northing);
            constantMultiple.northing = (ptList[j].easting - ptList[i].easting) / (ptList[j].northing - ptList[i].northing);
            calcList.append(constantMultiple);
        }
    }

    areaHectare = mf->locale.toString((double)(area * 0.0001),'g',1) + " " + "Ha";
    areaAcre = mf->locale.toString((double)(area * 0.000247105),'g', 1) + " " + "Ac";
}

bool CBoundary::isPrePointInPolygon(Vec2 testPoint)
{
    if (calcList.size() < 10) return false;
    int j = ptList.size() - 1;
    bool oddNodes = false;
    //vec2 testPoint = new vec2(mf.toolPos.easting, mf.toolPos.northing);

    //test against the constant and multiples list the test point
    for (int i = 0; i < ptList.size(); j = i++)
    {
        if ( (ptList[i].northing < testPoint.northing && ptList[j].northing >= testPoint.northing) ||
             (ptList[j].northing < testPoint.northing && ptList[i].northing >= testPoint.northing) )
        {
            oddNodes ^= (testPoint.northing * calcList[i].northing + calcList[i].easting < testPoint.easting);
        }
    }
    return oddNodes; //true means inside.
}

void CBoundary::drawBoundaryLine(QOpenGLContext *glContext)
{
    QOpenGLFunctions_1_1 *gl = glContext->versionFunctions<QOpenGLFunctions_1_1>();

    ////draw the perimeter line so far
    int ptCount = ptList.size();
    if (ptCount < 1) return;
    gl->glLineWidth(4);
    gl->glColor3f(0.98f, 0.2f, 0.60f);
    gl->glBegin(GL_LINE_STRIP);
    for (int h = 0; h < ptCount; h++)
        gl->glVertex3d(ptList[h].easting, ptList[h].northing, 0);
    gl->glEnd();

    //the "close the loop" line
    gl->glLineWidth(4);
    gl->glColor3f(0.9f, 0.32f, 0.70f);
    gl->glBegin(GL_LINE_STRIP);
    gl->glVertex3d(ptList[ptCount - 1].easting, ptList[ptCount - 1].northing, 0);
    gl->glVertex3d(ptList[0].easting, ptList[0].northing, 0);
    gl->glEnd();
}

//draw a blue line in the back buffer for section control over boundary line
void CBoundary::drawBoundaryLineOnBackBuffer(QOpenGLContext *glContext)
{
    QOpenGLFunctions_1_1 *gl = glContext->versionFunctions<QOpenGLFunctions_1_1>();

    ////draw the perimeter line so far
    int ptCount = ptList.size();
    if (ptCount < 1) return;
    gl->glLineWidth(4);
    gl->glColor3f(0.0f, 0.99f, 0.0f);
    gl->glBegin(GL_LINE_STRIP);
    for (int h = 0; h < ptCount; h++)
        gl->glVertex3d(ptList[h].easting, ptList[h].northing, 0);
    gl->glEnd();

    //the "close the loop" line
    gl->glLineWidth(4);
    gl->glColor3f(0.0f, 0.990f, 0.0f);
    gl->glBegin(GL_LINE_STRIP);
    gl->glVertex3d(ptList[ptCount - 1].easting, ptList[ptCount - 1].northing, 0);
    gl->glVertex3d(ptList[0].easting, ptList[0].northing, 0);
    gl->glEnd();
}

void CBoundary::calculateBoundaryArea()
{
    int ptCount = ptList.size();
    if (ptCount < 1) return;

    area = 0;         // Accumulates area in the loop
    int j = ptCount - 1;  // The last vertex is the 'previous' one to the first

    for (int i = 0; i < ptCount; j = i++)
    {
        area = area + (ptList[j].easting + ptList[i].easting) * (ptList[j].northing - ptList[i].northing);
    }
    area = fabs(area / 2);
}

//the non precalculated version
bool CBoundary::isPointInPolygon()
{
    bool result = false;
    int j = ptList.size() - 1;
    Vec2 testPoint (mf->pn->easting, mf->pn->northing);

    if (j < 10) return true;

    for (int i = 0; i < ptList.size(); j = i++)
    {
        //TODO: reformat this expression and make sure it's right
        result ^= ((ptList[i].northing > testPoint.northing) ^ (ptList[j].northing > testPoint.northing)) &&
            testPoint.easting < ((ptList[j].easting - ptList[i].easting) *
            (testPoint.northing - ptList[i].northing) / (ptList[j].northing - ptList[i].northing) + ptList[i].easting);
    }
    return result;
}

