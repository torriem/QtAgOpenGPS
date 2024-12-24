#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QVector>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include "glutils.h"
#include "ctrack.h"
#include "cvehicle.h"
#include "glm.h"
#include "cabcurve.h"
#include "cabline.h"
#include "aogproperty.h"
#include "cyouturn.h"
#include "cboundary.h"
#include "ctram.h"
#include "ccamera.h"
#include "cahrs.h"
#include "cguidance.h"

CTrk::CTrk()
{
    heading = 3;
}

CTrack::CTrack(QObject* parent) : QAbstractListModel(parent)
{
    // Initialize role names
    m_roleNames[index] = "index";
    m_roleNames[NameRole] = "name";
    m_roleNames[IsVisibleRole] = "isVisible";
    m_roleNames[ModeRole] = "mode";
    m_roleNames[ptA] = "ptA";
    m_roleNames[ptB] = "ptB";
    m_roleNames[endPtA] = "endPtA";
    m_roleNames[endPtB] = "endPtB";
    m_roleNames[nudgeDistance] = "nudgeDistance";

    setIdx(-1);
}

int CTrack::FindClosestRefTrack(Vec3 pivot, const CVehicle &vehicle)
{
    if (idx < 0 || gArr.count() == 0) return -1;

    //only 1 track
    if (gArr.count() == 1) return idx;

    int trak = -1;
    int cntr = 0;

    //Count visible
    for (int i = 0; i < gArr.count(); i++)
    {
        if (gArr[i].isVisible)
        {
            cntr++;
            trak = i;
        }
    }

    //only 1 track visible of the group
    if (cntr == 1) return trak;

    //no visible tracks
    if (cntr == 0) return -1;

    //determine if any aligned reasonably close
    QVector<bool> isAlignedArr;
    for (int i = 0; i < gArr.count(); i++)
    {
        if (gArr[i].mode == (int)TrackMode::Curve) isAlignedArr[i] = true;
        else
        {
            double diff = M_PI - fabs(fabs(pivot.heading - gArr[i].heading) - M_PI);
            if ((diff < 1) || (diff > 2.14))
                isAlignedArr[i] = true;
            else
                isAlignedArr[i] = false;
        }
    }

    double minDistA = glm::DOUBLE_MAX;
    double dist;

    Vec2 endPtA, endPtB;

    for (int i = 0; i < gArr.count(); i++)
    {
        if (!isAlignedArr[i]) continue;
        if (!gArr[i].isVisible) continue;

        if (gArr[i].mode == (int)TrackMode::AB)
        {
            double abHeading = gArr[i].heading;

            endPtA.easting = gArr[i].ptA.easting - (sin(abHeading) * 2000);
            endPtA.northing = gArr[i].ptA.northing - (cos(abHeading) * 2000);

            endPtB.easting = gArr[i].ptB.easting + (sin(abHeading) * 2000);
            endPtB.northing = gArr[i].ptB.northing + (cos(abHeading) * 2000);

            //x2-x1
            double dx = endPtB.easting - endPtA.easting;
            //z2-z1
            double dy = endPtB.northing - endPtA.northing;

            dist = ((dy * vehicle.steerAxlePos.easting) - (dx * vehicle.steerAxlePos.northing) +
                    (endPtB.easting * endPtA.northing) - (endPtB.northing * endPtA.easting))
                   / sqrt((dy * dy) + (dx * dx));

            dist *= dist;

            if (dist < minDistA)
            {
                minDistA = dist;
                trak = i;
            }
        }
        else
        {
            for (int j = 0; j < gArr[i].curvePts.count(); j ++)
            {

                dist = glm::DistanceSquared(gArr[i].curvePts[j], pivot);

                if (dist < minDistA)
                {
                    minDistA = dist;
                    trak = i;
                }
            }
        }
    }

    return trak;
}

void CTrack::SwitchToClosestRefTrack(Vec3 pivot, const CVehicle &vehicle)
{
    int new_idx;

    new_idx = FindClosestRefTrack(pivot, vehicle);
    if (new_idx >= 0 && new_idx != idx) {
        setIdx(new_idx);
        curve.isCurveValid = false;
        ABLine.isABValid = false;
    }
}

void CTrack::NudgeTrack(double dist)
{
    if (idx > -1)
    {
        if (gArr[idx].mode == (int)TrackMode::AB)
        {
            ABLine.isABValid = false;
            ABLine.lastSecond = 0;
            gArr[idx].nudgeDistance += ABLine.isHeadingSameWay ? dist : -dist;
        }
        else
        {
            curve.isCurveValid = false;
            curve.lastHowManyPathsAway = 98888;
            curve.lastSecond = 0;
            gArr[idx].nudgeDistance += curve.isHeadingSameWay ? dist : -dist;
        }

        //if (gArr[idx].nudgeDistance > 0.5 * mf.tool.width) gArr[idx].nudgeDistance -= mf.tool.width;
        //else if (gArr[idx].nudgeDistance < -0.5 * mf.tool.width) gArr[idx].nudgeDistance += mf.tool.width;
    }
}

void CTrack::NudgeDistanceReset()
{
    if (idx > -1 && gArr.count() > 0)
    {
        if (gArr[idx].mode == (int)TrackMode::AB)
        {
            ABLine.isABValid = false;
            ABLine.lastSecond = 0;
        }
        else
        {
            curve.isCurveValid = false;
            curve.lastHowManyPathsAway = 98888;
            curve.lastSecond = 0;
        }

        gArr[idx].nudgeDistance = 0;
    }
}

void CTrack::SnapToPivot()
{
    //if (isBtnGuidanceOn)

    if (idx > -1)
    {
        if (gArr[idx].mode == (int)(TrackMode::AB))
        {
            NudgeTrack(ABLine.distanceFromCurrentLinePivot);

        }
        else
        {
            NudgeTrack(curve.distanceFromCurrentLinePivot);
        }

    }
}

void CTrack::NudgeRefTrack(double dist)
{
    if (idx > -1)
    {
        if (gArr[idx].mode == (int)TrackMode::AB)
        {
            ABLine.isABValid = false;
            ABLine.lastSecond = 0;
            NudgeRefABLine( ABLine.isHeadingSameWay ? dist : -dist);
        }
        else
        {
            curve.isCurveValid = false;
            curve.lastHowManyPathsAway = 98888;
            curve.lastSecond = 0;
            NudgeRefCurve( curve.isHeadingSameWay ? dist : -dist);
        }
    }
}

void CTrack::NudgeRefABLine(double dist)
{
    double head = gArr[idx].heading;

    gArr[idx].ptA.easting += (sin(head+glm::PIBy2) * (dist));
    gArr[idx].ptA.northing += (cos(head + glm::PIBy2) * (dist));

    gArr[idx].ptB.easting += (sin(head + glm::PIBy2) * (dist));
    gArr[idx].ptB.northing += (cos(head + glm::PIBy2) * (dist));
}

void CTrack::NudgeRefCurve(double distAway)
{
    curve.isCurveValid = false;
    curve.lastHowManyPathsAway = 98888;
    curve.lastSecond = 0;

    QVector<Vec3> curList;

    double distSqAway = (distAway * distAway) - 0.01;
    Vec3 point;

    for (int i = 0; i < gArr[idx].curvePts.count(); i++)
    {
        point = Vec3(
            gArr[idx].curvePts[i].easting + (sin(glm::PIBy2 + gArr[idx].curvePts[i].heading) * distAway),
            gArr[idx].curvePts[i].northing + (cos(glm::PIBy2 + gArr[idx].curvePts[i].heading) * distAway),
            gArr[idx].curvePts[i].heading);
        bool Add = true;

        for (int t = 0; t < gArr[idx].curvePts.count(); t++)
        {
            double dist = ((point.easting - gArr[idx].curvePts[t].easting) * (point.easting - gArr[idx].curvePts[t].easting))
                          + ((point.northing - gArr[idx].curvePts[t].northing) * (point.northing - gArr[idx].curvePts[t].northing));
            if (dist < distSqAway)
            {
                Add = false;
                break;
            }
        }

        if (Add)
        {
            if (curList.count() > 0)
            {
                double dist = ((point.easting - curList[curList.count() - 1].easting) * (point.easting - curList[curList.count() - 1].easting))
                              + ((point.northing - curList[curList.count() - 1].northing) * (point.northing - curList[curList.count() - 1].northing));
                if (dist > 1.0)
                    curList.append(point);
            }
            else curList.append(point);
        }
    }

    int cnt = curList.count();
    if (cnt > 6)
    {
        QVector<Vec3> arr = curList;
        curList.clear();

        for (int i = 0; i < (arr.count() - 1); i++)
        {
            arr[i].heading = atan2(arr[i + 1].easting - arr[i].easting, arr[i + 1].northing - arr[i].northing);
            if (arr[i].heading < 0) arr[i].heading += glm::twoPI;
            if (arr[i].heading >= glm::twoPI) arr[i].heading -= glm::twoPI;
        }

        arr[arr.count() - 1].heading = arr[arr.count() - 2].heading;

        //replace the array
        cnt = arr.count();
        double distance;
        double spacing = 1.2;

        //add the first point of loop - it will be p1
        curList.append(arr[0]);

        for (int i = 0; i < cnt - 3; i++)
        {
            // add p2
            curList.append(arr[i + 1]);

            distance = glm::Distance(arr[i + 1], arr[i + 2]);

            if (distance > spacing)
            {
                int loopTimes = (int)(distance / spacing + 1);
                for (int j = 1; j < loopTimes; j++)
                {
                    Vec3 pos(glm::Catmull(j / (double)(loopTimes), arr[i], arr[i + 1], arr[i + 2], arr[i + 3]));
                    curList.append(pos);
                }
            }
        }

        curList.append(arr[cnt - 2]);
        curList.append(arr[cnt - 1]);

        curve.CalculateHeadings(curList);

        gArr[idx].curvePts = curList;
        //gArr[idx].curvePts.clear();

        //for (auto item: curList)
        //{
        //    gArr[idx].curvePts.append(new vec3(item));
        //}

        //for (int i = 0; i < cnt; i++)
        //{
        //    arr[i].easting += cos(arr[i].heading) * (dist);
        //    arr[i].northing -= sin(arr[i].heading) * (dist);
        //    gArr[idx].curvePts.append(arr[i]);
        //}
    }
}

void CTrack::DrawTrackNew(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, const CCamera &camera)
{
    if (idx >= 0) {
        if (gArr[idx].mode == TrackMode::AB)
            ABLine.DrawABLineNew(gl, mvp, camera);
        else if (gArr[idx].mode == TrackMode::Curve)
            curve.DrawCurveNew(gl, mvp);
    }
}

void CTrack::DrawTrack(QOpenGLFunctions *gl,
                       const QMatrix4x4 &mvp,
                       bool isFontOn,
                       CYouTurn &yt,
                       const CCamera &camera,
                       const CGuidance &gyd)
{
    if (idx >= 0) {
        if (gArr[idx].mode == TrackMode::AB)
            ABLine.DrawABLines(gl, mvp, isFontOn,gArr[idx], yt, camera, gyd);
        else if (gArr[idx].mode == TrackMode::Curve)
            curve.DrawCurve(gl, mvp, isFontOn, gArr[idx], yt, camera);
    }
}

void CTrack::DrawTrackGoalPoint(QOpenGLFunctions *gl,
                                const QMatrix4x4 &mvp)
{
    GLHelperOneColor gldraw1;
    QColor color;

    if (idx >= 0) {
        color.setRgbF(0.98, 0.98, 0.098);
        if (gArr[idx].mode == TrackMode::AB) {
            gldraw1.append(QVector3D(ABLine.goalPointAB.easting, ABLine.goalPointAB.northing, 0));
            gldraw1.draw(gl,mvp,QColor::fromRgbF(0,0,0),GL_POINTS,16);
            gldraw1.draw(gl,mvp,color,GL_POINTS,10);
        } else if (gArr[idx].mode == TrackMode::Curve) {
            gldraw1.append(QVector3D(curve.goalPointCu.easting, curve.goalPointCu.northing, 0));
            gldraw1.draw(gl,mvp,QColor::fromRgbF(0,0,0),GL_POINTS,16);
            gldraw1.draw(gl,mvp,color,GL_POINTS,10);
        }
    }
}

void CTrack::BuildCurrentLine(Vec3 pivot, double secondsSinceStart,
                              bool isAutoSteerBtnOn,
                              int &makeUTurnCounter,
                              CYouTurn &yt,
                              CVehicle &vehicle,
                              const CBoundary &bnd,
                              const CAHRS &ahrs,
                              CGuidance &gyd,
                              CNMEA &pn)
{
    if (gArr.count() > 0 && idx > -1)
    {
        if (gArr[idx].mode == TrackMode::AB)
        {
            ABLine.BuildCurrentABLineList(pivot,secondsSinceStart,gArr[idx],yt,vehicle);

            ABLine.GetCurrentABLine(pivot, vehicle.steerAxlePos,isAutoSteerBtnOn,vehicle,yt,ahrs,gyd,pn,makeUTurnCounter);
        }
        else
        {
            //build new current ref line if required
            curve.BuildCurveCurrentList(pivot, secondsSinceStart,vehicle,gArr[idx],bnd,yt);

            curve.GetCurrentCurveLine(pivot, vehicle.steerAxlePos,isAutoSteerBtnOn,vehicle,gArr[idx],yt,ahrs,gyd,pn,makeUTurnCounter);
        }
    }
    emit howManyPathsAwayChanged(); //notify QML property is changed
}

void CTrack::ResetCurveLine()
{
    if (idx >=0 && gArr[idx].mode == TrackMode::Curve) {
        curve.curList.clear();
        setIdx(-1);
    }
}

void CTrack::AddPathPoint(Vec3 point)
{
    if (curve.isMakingCurve) {
        curve.desList.append(point);
    } else if (ABLine.isMakingABLine) {
        ABLine.desHeading = atan2(point.easting - ABLine.desPtA.easting,
                                  point.northing - ABLine.desPtA.northing);

        ABLine.desLineEndA.easting = ABLine.desPtA.easting - (sin(ABLine.desHeading) * 1000);
        ABLine.desLineEndA.northing = ABLine.desPtA.northing - (cos(ABLine.desHeading) * 1000);

        ABLine.desLineEndB.easting = ABLine.desPtA.easting + (sin(ABLine.desHeading) * 1000);
        ABLine.desLineEndB.northing = ABLine.desPtA.northing + (cos(ABLine.desHeading) * 1000);
    }
}

int CTrack::getHowManyPathsAway()
{
    if (idx >= 0) {
        if (gArr[idx].mode == TrackMode::AB)
            return ABLine.howManyPathsAway;
        else
            return curve.howManyPathsAway;
    }

    return 0;
}

void CTrack::setIdx(int new_idx)
{
    if (new_idx < gArr.count()) {
        idx = new_idx;
        emit idxChanged();
        emit modeChanged();
    }
}

int CTrack::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return gArr.size();
}

QVariant CTrack::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= gArr.size()) {
        return QVariant();
    }

    const CTrk &trk = gArr.at(row);
    qDebug() << row << role << trk.name;
    switch(role) {
    case RoleNames::index:
        return row;
    case RoleNames::NameRole:
        return trk.name;
    case RoleNames::ModeRole:
        return trk.mode;
    case RoleNames::IsVisibleRole:
        return trk.isVisible;
    case RoleNames::ptA:
        return QVector2D(trk.ptA.easting, trk.ptA.northing);
    case RoleNames::ptB:
        return QVector2D(trk.ptB.easting, trk.ptB.northing);
    case RoleNames::endPtA:
        return QVector2D(trk.endPtA.easting, trk.endPtA.northing);
    case RoleNames::endPtB:
        return QVector2D(trk.endPtB.easting, trk.endPtB.northing);
    case RoleNames::nudgeDistance:
        return trk.nudgeDistance;
    }

    return QVariant();
}

QHash<int, QByteArray> CTrack::roleNames() const
{
    return m_roleNames;
}
