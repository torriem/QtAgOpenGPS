#include <QtGlobal>
#include "cvehicle.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QRgb>
#include "aogproperty.h"
#include "ccamera.h"
#include "cboundary.h"
#include "ctool.h"
#include "glm.h"
#include "glutils.h"
#include "cnmea.h"
#include "ccamera.h"
#include "cabline.h"
#include "cabcurve.h"
#include "ccontour.h"
#include "ctrack.h"

QRect find_bounding_box(int viewport_height, QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4) {
    float x_min = glm::FLOAT_MAX;
    float x_max = glm::FLOAT_MIN;
    float y_min = glm::FLOAT_MAX;
    float y_max = glm::FLOAT_MIN;

    if(p1.x() < x_min) x_min = p1.x();
    if(p1.x() > x_max) x_max = p1.x();
    if(p1.y() < y_min) y_min = p1.y();
    if(p1.y() > y_max) y_max = p1.y();

    if(p2.x() < x_min) x_min = p2.x();
    if(p2.x() > x_max) x_max = p2.x();
    if(p2.y() < y_min) y_min = p2.y();
    if(p2.y() > y_max) y_max = p2.y();

    if(p3.x() < x_min) x_min = p3.x();
    if(p3.x() > x_max) x_max = p3.x();
    if(p3.y() < y_min) y_min = p3.y();
    if(p3.y() > y_max) y_max = p3.y();

    if(p4.x() < x_min) x_min = p4.x();
    if(p4.x() > x_max) x_max = p4.x();
    if(p4.y() < y_min) y_min = p4.y();
    if(p4.y() > y_max) y_max = p4.y();

    return QRect(x_min, viewport_height - y_max, x_max-x_min, y_max-y_min);
}

void CVehicle::loadSettings()
{
    isPivotBehindAntenna = property_setVehicle_isPivotBehindAntenna;
    antennaHeight = property_setVehicle_antennaHeight;
    antennaPivot = property_setVehicle_antennaPivot;
    antennaOffset = property_setVehicle_antennaOffset;

    wheelbase = property_setVehicle_wheelbase;
    isSteerAxleAhead = property_setVehicle_isSteerAxleAhead;

    slowSpeedCutoff = property_setVehicle_slowSpeedCutoff;

    goalPointLookAhead = property_setVehicle_goalPointLookAhead;
    goalPointLookAheadHold = property_setVehicle_goalPointLookAheadHold;
    goalPointLookAheadMult = property_setVehicle_goalPointLookAheadMult;

    stanleyDistanceErrorGain = property_stanleyDistanceErrorGain;
    stanleyHeadingErrorGain = property_stanleyHeadingErrorGain;

    maxAngularVelocity = property_setVehicle_maxAngularVelocity;
    maxSteerAngle = property_setVehicle_maxSteerAngle;


    trackWidth = property_setVehicle_trackWidth;

    stanleyIntegralGainAB = property_stanleyIntegralGainAB;
    stanleyIntegralDistanceAwayTriggerAB = property_stanleyIntegralDistanceAwayTriggerAB;

    purePursuitIntegralGain = property_purePursuitIntegralGainAB;
    vehicleType = property_setVehicle_vehicleType;

    hydLiftLookAheadTime = property_setVehicle_hydraulicLiftLookAhead;
    panicStopSpeed = property_setVehicle_panicStopSpeed;


    //how far from line before it becomes Hold
    modeXTE = property_setAS_ModeXTE;

    //how long before hold is activated
    modeTime = property_setAS_ModeTime;

    functionSpeedLimit = property_setAS_functionSpeedLimit;
    maxSteerSpeed = property_setAS_maxSteerSpeed;
    minSteerSpeed = property_setAS_minSteerSpeed;

    uturnCompensation = property_setAS_uTurnCompensation;
}

CVehicle::CVehicle(QObject *parent) : QObject(parent)
{
    isInFreeDriveMode = false;
    loadSettings();

}

//called from various Classes, always needs current speed
double CVehicle::UpdateGoalPointDistance()
{
    double xTE = fabs(modeActualXTE);

    //how far should goal point be away  - speed * seconds * kmph -> m/s then limit min value
    double goalPointDistance = avgSpeed * goalPointLookAhead * 0.05 * goalPointLookAheadMult;
    goalPointDistance += goalPointLookAhead;

    if (xTE < (modeXTE))
    {
        if (modeTimeCounter > modeTime * 10)
        {
            goalPointDistance = avgSpeed * goalPointLookAheadHold * 0.05 * goalPointLookAheadMult;
            goalPointDistance += goalPointLookAheadHold;
        }
        else
        {
            modeTimeCounter++;
        }
    }
    else
    {
        modeTimeCounter = 0;
    }

    if (goalPointDistance < 1) goalPointDistance = 1;
    goalDistance = goalPointDistance;

    return goalPointDistance;


}

void CVehicle::DrawVehicle(QOpenGLFunctions *gl, QMatrix4x4 modelview,
                           QMatrix4x4 projection,
                           double steerAngle,
                           bool isFirstHeadingSet,
                           QRect viewport,
                           const CCamera &camera,
                           const CTool &tool,
                           CBoundary &bnd
                           )
{
    //draw vehicle
    modelview.rotate(glm::toDegrees(-fixHeading), 0.0, 0.0, 1.0);

    GLHelperColors glcolors;
    GLHelperOneColor gldraw;
    GLHelperTexture gltex;
    //VertexTexcoord tc;

    QColor color;
    ColorVertex cv;

    QMatrix4x4 savedModelView = modelview;
    QMatrix4x4 mvp = projection*modelview;

    float vehicleOpacity = 1.0f; //TODO: mf.vehicleOpacity

    QVector3D p1;
    QVector3D p2;
    QVector3D p3;
    QVector3D p4;
    QVector3D s;
    int x,w;

    s = QVector3D(0,0,0);
    p1 = s.project(modelview, projection, viewport);
    pivot_axle_xy = QPoint(p1.x(), p1.y());

    if (isFirstHeadingSet && !tool.isToolFrontFixed)
    {
        if (!tool.isToolRearFixed)
        {
            //draw the rigid hitch
            color.setRgbF(0, 0, 0);
            gldraw.append(QVector3D(0, tool.hitchLength, 0));
            gldraw.append(QVector3D(0, 0, 0));
            gldraw.draw(gl,mvp,color,GL_LINES,4);

            gldraw.clear();
            color.setRgbF(1.237f, 0.037f, 0.0397f);
            gldraw.append(QVector3D(0, tool.hitchLength, 0));
            gldraw.append(QVector3D(0, 0, 0));
            gldraw.draw(gl,mvp,color,GL_LINES,1);
        }
        else
        {
            //draw the rigid hitch
            color.setRgbF(0, 0, 0);
            gldraw.append(QVector3D(-0.35, tool.hitchLength, 0));
            gldraw.append(QVector3D(-0.350, 0, 0));
            gldraw.append(QVector3D(0.35, tool.hitchLength, 0));
            gldraw.append(QVector3D(0.350, 0, 0));
            gldraw.draw(gl,mvp,color,GL_LINES,4);

            color.setRgbF(1.237f, 0.037f, 0.0397f);
            gldraw.append(QVector3D(-0.35, tool.hitchLength, 0));
            gldraw.append(QVector3D(-0.35, 0, 0));
            gldraw.append(QVector3D(0.35, tool.hitchLength, 0));
            gldraw.append(QVector3D(0.35, 0, 0));
            gldraw.draw(gl,mvp,color,GL_LINES,1);
        }
    }

    //draw the vehicle Body

    if (!isFirstHeadingSet)
    {

        //using texture 14, Textures::QUESTION_MARK
        gltex.append( { QVector3D(5,5,0), QVector2D(1,0) } );
        gltex.append( { QVector3D(1,5,0), QVector2D(0,0) } );
        gltex.append( { QVector3D(5,1,0), QVector2D(1,1) } );
        gltex.append( { QVector3D(1,1,0), QVector2D(0,1) } );
        gltex.draw(gl,mvp,Textures::QUESTION_MARK,GL_TRIANGLE_STRIP,false);

    }

    //3 vehicle types  tractor=0 harvestor=1 4wd=2
    if (
        property_setDisplay_isVehicleImage)
    {
        s = QVector3D(0,0,0); //should be pivot axle
        p1 = s.project(modelview, projection, viewport);
        pivot_axle_xy = QPoint(p1.x(), viewport.height() - p1.y());

        if (vehicleType == 0)
        {
            //vehicle body
            //texture is 13, or Textures::TRACTOR, Wheel is 15

            //TODO: color.setRgbF(mf.vehicleColor.R, mf.vehicleColor.G, mf.vehicleColor.B, mf.vehicleOpacityByte);

            double leftAckerman, rightAckerman;

            if (steerAngle < 0)
            {
                leftAckerman = 1.25 * -steerAngle;
                rightAckerman = -steerAngle;
            }
            else
            {
                leftAckerman = -steerAngle;
                rightAckerman = 1.25 * -steerAngle;
            }

            //tractor body
            gltex.clear();
            gltex.append( { QVector3D(trackWidth, wheelbase * 1.5, 0.0),   QVector2D(1, 0) } );
            gltex.append( { QVector3D(-trackWidth, wheelbase * 1.5, 0.0),  QVector2D(0, 0) } );
            gltex.append( { QVector3D(trackWidth, -wheelbase * 0.5, 0.0),  QVector2D(1, 1) } );
            gltex.append( { QVector3D(-trackWidth, -wheelbase * 0.5, 0.0), QVector2D(0, 1) } );
            gltex.draw(gl,mvp,Textures::TRACTOR,GL_TRIANGLE_STRIP,false); //TODO: colorize

            s = QVector3D(-trackWidth, wheelbase * 1.5, 0.0); //front left corner
            p1 = s.project(modelview,projection, viewport);

            s = QVector3D(trackWidth, wheelbase * 1.5, 0.0); //front right corner
            p2 = s.project(modelview, projection, viewport);

            s = QVector3D(-trackWidth, -wheelbase * 0.5, 0.0); //rear left corne
            p3 = s.project(modelview, projection, viewport);

            s = QVector3D(trackWidth, -wheelbase * 0.5, 0.0); //rear right corner
            p4 = s.project(modelview, projection, viewport);

            bounding_box = find_bounding_box(viewport.height(),p1, p2, p3, p4);

            //right wheel
            //push modelview... nop because savedModelView already has a copy

            gltex.clear();
            gltex.append( { QVector3D(trackWidth * 0.5, wheelbase * 0.75, 0.0),   QVector2D(1, 0) } ); //Top Right
            gltex.append( { QVector3D(-trackWidth * 0.5, wheelbase * 0.75, 0.0),  QVector2D(0, 0) } ); //Top Left
            gltex.append( { QVector3D(trackWidth * 0.5, -wheelbase * 0.75, 0.0),  QVector2D(1, 1) } ); //Bottom Right
            gltex.append( { QVector3D(-trackWidth * 0.5, -wheelbase * 0.75, 0.0), QVector2D(0, 1) } ); //Bottom Left

            modelview.translate(trackWidth * 0.5, wheelbase, 0);
            modelview.rotate(rightAckerman, 0, 0, 1);

            gltex.draw(gl,projection*modelview,Textures::FRONT_WHEELS,GL_TRIANGLE_STRIP,false); //TODO: colorize

            modelview = savedModelView; //pop matrix

            //Left Wheel
            //push modelview, nop because savedModelView already has a copy

            gltex.clear();
            gltex.append( { QVector3D(trackWidth * 0.5, wheelbase * 0.75, 0.0),   QVector2D(1, 0) } ); //Top Right
            gltex.append( { QVector3D(-trackWidth * 0.5, wheelbase * 0.75, 0.0),  QVector2D(0, 0) } ); //Top Left
            gltex.append( { QVector3D(trackWidth * 0.5, -wheelbase * 0.75, 0.0),  QVector2D(1, 1) } ); //Bottom Right
            gltex.append( { QVector3D(-trackWidth * 0.5, -wheelbase * 0.75, 0.0), QVector2D(0, 1) } ); //Bottom Left

            modelview.translate(-trackWidth * 0.5, wheelbase, 0);
            modelview.rotate(leftAckerman, 0, 0, 1);

            gltex.draw(gl,projection*modelview,Textures::FRONT_WHEELS,GL_TRIANGLE_STRIP,false); //TODO: colorize

            modelview = savedModelView; //pop matrix
        }
        else if (vehicleType == 1) //Harvester
        {
            //TODO: color.setRgbF(0.078, 0.078, 0.078, mf.vehicleOpacityByte / 255.0);

            double leftAckerman, rightAckerman;

            if (steerAngle < 0)
            {
                leftAckerman = 1.25 * steerAngle;
                rightAckerman = steerAngle;
            }
            else
            {
                leftAckerman = steerAngle;
                rightAckerman = 1.25 * steerAngle;
            }

            //right wheel
            //push modelview... nop because savedModelView already has a copy

            gltex.clear();
            gltex.append( { QVector3D(trackWidth * 0.25, wheelbase * 0.5, 0.0),   QVector2D(1, 0) } ); //Top Right
            gltex.append( { QVector3D(-trackWidth * 0.25, wheelbase * 0.5, 0.0),  QVector2D(0, 0) } ); //Top Left
            gltex.append( { QVector3D(trackWidth * 0.25, -wheelbase * 0.5, 0.0),  QVector2D(1, 1) } ); //Bottom Right
            gltex.append( { QVector3D(-trackWidth * 0.25, -wheelbase * 0.5, 0.0), QVector2D(0, 1) } ); //Bottom Left

            modelview.translate(trackWidth * 0.5, -wheelbase, 0);
            modelview.rotate(rightAckerman, 0, 0, 1);

            gltex.draw(gl,projection*modelview,Textures::FRONT_WHEELS,GL_TRIANGLE_STRIP,false); //TODO: colorize

            modelview = savedModelView; //pop matrix

            //Left Wheel
            //push modelview, nop because savedModelView already has a copy

            gltex.clear();
            gltex.append( { QVector3D(trackWidth * 0.25, wheelbase * 0.5, 0.0),   QVector2D(1, 0) } ); //Top Right
            gltex.append( { QVector3D(-trackWidth * 0.25, wheelbase * 0.5, 0.0),  QVector2D(0, 0) } ); //Top Left
            gltex.append( { QVector3D(trackWidth * 0.25, -wheelbase * 0.5, 0.0),  QVector2D(1, 1) } ); //Bottom Right
            gltex.append( { QVector3D(-trackWidth * 0.25, -wheelbase * 0.5, 0.0), QVector2D(0, 1) } ); //Bottom Left

            modelview.translate(-trackWidth * 0.5, -wheelbase, 0);
            modelview.rotate(leftAckerman, 0, 0, 1);

            gltex.draw(gl,projection*modelview,Textures::FRONT_WHEELS,GL_TRIANGLE_STRIP,false); //TODO: colorize

            modelview = savedModelView; //pop matrix
            //harvester body
            //TODO colorize
            //color.setRgbF(mf.vehicleColor.R, mf.vehicleColor.G, mf.vehicleColor.B, mf.vehicleOpacityByte);
            gltex.clear();
            gltex.append( { QVector3D(trackWidth, wheelbase * 1.5, 0.0),   QVector2D(1, 0) } );
            gltex.append( { QVector3D(-trackWidth, wheelbase * 1.5, 0.0),  QVector2D(0, 0) } );
            gltex.append( { QVector3D(trackWidth, -wheelbase * 1.5, 0.0),  QVector2D(1, 1) } );
            gltex.append( { QVector3D(-trackWidth, -wheelbase * 1.5, 0.0), QVector2D(0, 1) } );
            gltex.draw(gl,mvp,Textures::HARVESTER,GL_TRIANGLE_STRIP,false); //TODO: colorize

            s = QVector3D(-trackWidth, wheelbase * 1.5, 0.0); //front left corner
            p1 = s.project(modelview,projection, viewport);

            s = QVector3D(trackWidth, wheelbase * 1.5, 0.0); //front right corner
            p2 = s.project(modelview, projection, viewport);

            s = QVector3D(-trackWidth, -wheelbase * 1.5, 0.0); //rear left corne
            p3 = s.project(modelview, projection, viewport);

            s = QVector3D(trackWidth, -wheelbase * 1.5, 0.0); //rear right corner
            p4 = s.project(modelview, projection, viewport);

            bounding_box = find_bounding_box(viewport.height(),p1, p2, p3, p4);
        }
        else if (vehicleType == 2) //4WD tractor, articulated
        {

            double modelSteerAngle = 0.5 * steerAngle;
            //TODO: color.setRgbF(mf.vehicleColor.R, mf.vehicleColor.G, mf.vehicleColor.B, mf.vehicleOpacityByte);

            //tractor rear
            //push modelview... nop because savedModelView already has a copy

            gltex.clear();
            gltex.append( { QVector3D(trackWidth, wheelbase * 0.65, 0.0),   QVector2D(1, 0) } ); //Top Right
            gltex.append( { QVector3D(-trackWidth, wheelbase * 0.65, 0.0),  QVector2D(0, 0) } ); //Top Left
            gltex.append( { QVector3D(trackWidth, -wheelbase * 0.65, 0.0),  QVector2D(1, 1) } ); //Bottom Right
            gltex.append( { QVector3D(-trackWidth, -wheelbase * 0.65, 0.0), QVector2D(0, 1) } ); //Bottom Left

            modelview.translate(0, -wheelbase * 0.5, 0);
            modelview.rotate(modelSteerAngle, 0, 0, 1);

            gltex.draw(gl,projection*modelview,Textures::TRACTOR_4WD_REAR,GL_TRIANGLE_STRIP,false); //TODO: colorize

            s = QVector3D(-trackWidth, -wheelbase * 0.65, 0.0); //rear left corner
            p3 = s.project(modelview, projection, viewport);

            s = QVector3D(trackWidth, -wheelbase * 0.65, 0.0); //rear right corner
            p4 = s.project(modelview, projection, viewport);

            modelview = savedModelView; //pop matrix

            //tractor front
            //push modelview, nop because savedModelView already has a copy

            gltex.clear();
            gltex.append( { QVector3D(trackWidth, wheelbase * 0.65, 0.0),   QVector2D(1, 0) } ); //Top Right
            gltex.append( { QVector3D(-trackWidth, wheelbase * 0.65, 0.0),  QVector2D(0, 0) } ); //Top Left
            gltex.append( { QVector3D(trackWidth, -wheelbase * 0.65, 0.0),  QVector2D(1, 1) } ); //Bottom Right
            gltex.append( { QVector3D(-trackWidth, -wheelbase * 0.65, 0.0), QVector2D(0, 1) } ); //Bottom Left

            modelview.translate(0, wheelbase*0.5, 0);
            modelview.rotate(-modelSteerAngle, 0, 0, 1);

            gltex.draw(gl,projection*modelview,Textures::TRACTOR_4WD_FRONT,GL_TRIANGLE_STRIP,false); //TODO: colorize

            s = QVector3D(-trackWidth, wheelbase * 0.65, 0.0); //front left corner
            p1 = s.project(modelview,projection, viewport);
            s = QVector3D(-trackWidth, wheelbase * 0.65, 0.0); //front right corner
            p2 = s.project(modelview,projection, viewport);

            bounding_box = find_bounding_box(viewport.height(),p1, p2, p3, p4);

            modelview = savedModelView; //pop matrix
        }
    }
    else
    { //just draw a triangle
        color.setRgbF(1.2, 1.20, 0.0, vehicleOpacity);
        glcolors.clear();
        glcolors.append( { QVector3D(0, antennaPivot, -0.0), QVector4D(0.0, 1.20, 1.22, vehicleOpacity) } );
        glcolors.append( { QVector3D(0, wheelbase, 0.0),     QVector4D(1.220, 0.0, 1.2, vehicleOpacity) } );
        glcolors.append( { QVector3D(-1.0, -0, 0.0), QVector4D(1.220, 0.0, 1.2, vehicleOpacity) } );
        glcolors.append( { QVector3D(1.0, -0, 0.0),  QVector4D(1.220, 0.0, 1.2, vehicleOpacity) } );
        glcolors.draw(gl,mvp,GL_TRIANGLE_FAN,1.0f);

        gldraw.clear();
        color.setRgbF(0.12, 0.12, 0.12);
        gldraw.append(QVector3D(-1.0, 0, 0));
        gldraw.append(QVector3D(1.0, 0, 0));
        gldraw.append(QVector3D(0, wheelbase, 0));
        gldraw.draw(gl,mvp,color,GL_LINE_LOOP, 3.0f);

        s = QVector3D(0, wheelbase, 0); //front point
        p1 = s.project(modelview,projection, viewport);

        s = QVector3D(-1.0, 0, 0.0); //rear left corne
        p3 = s.project(modelview, projection, viewport);

        s = QVector3D(1.0, 0, 0.0); //rear right corner
        p4 = s.project(modelview, projection, viewport);

        bounding_box = find_bounding_box(viewport.height(), p1, p1, p3, p4);
    }

    if (camera.camSetDistance > -75 && isFirstHeadingSet)
    {
        //draw the bright antenna dot
        gldraw.clear();
        color.setRgbF(0,0,0);
        gldraw.append(QVector3D(-antennaOffset, antennaPivot, 0.1));
        gldraw.draw(gl,mvp,color,GL_POINTS,16.0f);

        gldraw.clear();
        color.setRgbF(0.2,0.98,0.98);
        gldraw.append(QVector3D(-antennaOffset, antennaPivot, 0.1));
        gldraw.draw(gl,mvp,color,GL_POINTS,10.0f);
    }

    if (bnd.isBndBeingMade)
    {
        glcolors.clear();

        gl->glLineWidth(2);
        cv.color = QVector4D(0.0, 1.270, 0.0, 1.0);

        if (bnd.isDrawRightSide)
        {
            cv.vertex = QVector3D(0.0, 0, 0);
            glcolors.append(cv);

            cv.color = QVector4D(1.270, 1.220, 0.20, 1.0);
            cv.vertex = QVector3D((double)bnd.createBndOffset, 0, 0);
            glcolors.append(cv);

            cv.vertex = QVector3D((double)bnd.createBndOffset*0.75, 0.25, 0);
            glcolors.append(cv);

            glcolors.draw(gl,mvp,GL_LINE_STRIP, 2);
        }

        //draw on left side
        else
        {
            cv.vertex = QVector3D(0.0, 0, 0);
            glcolors.append(cv);

            cv.color = QVector4D(1.270, 1.220, 0.20, 1.0);
            cv.vertex = QVector3D(-(double)bnd.createBndOffset, 0, 0);
            glcolors.append(cv);

            cv.vertex = QVector3D(-(double)bnd.createBndOffset*0.75, 0.25, 0);
            glcolors.append(cv);

            glcolors.draw(gl,mvp,GL_LINE_STRIP, 2);
        }
    }

    //Svenn Arrow

    if (property_setDisplay_isSvennArrowOn && camera.camSetDistance > -1000)
    {
        //double offs = mf.curve.distanceFromCurrentLinePivot * 0.3;
        double svennDist = camera.camSetDistance * -0.07;
        double svennWidth = svennDist * 0.22;

        gldraw.clear();
        color.setRgbF(1.2, 1.25, 0.10);
        gldraw.append(QVector3D(svennWidth, wheelbase + svennDist, 0.0));
        gldraw.append(QVector3D(0, wheelbase + svennWidth + 0.5 + svennDist, 0.0));
        gldraw.append(QVector3D(-svennWidth, wheelbase + svennDist, 0.0));

        gldraw.draw(gl,mvp,color,GL_LINE_STRIP,property_setDisplay_lineWidth);
    }

    /*
    // TODO: Track number and nudge offset need to be done in QML.
    // Probably will need CTrack to put this information in its model

    if(!(bool)property_setDisplay_topTrackNum && trk.idx > -1 && !ct.isContourBtnOn) {
        color.setRgbF(1.269, 1.25, 1.2510, 0.87); //?? why over 1.0?

        if (curve.isBtnCurveOn && (bool)ct.isContourBtnOn == false)
        {
            if (curve.howManyPathsAway == 0) {
                drawTextVehicle(camera, gl, mvp, 2, wheelbase+1, "0", 1,
                                true, color);
            } else if (curve.howManyPathsAway > 0) {
                drawTextVehicle(camera, gl, mvp, 2, wheelbase+1,
                                QString("%1").arg(curve.howManyPathsAway,0,'f',0), 1,
                                true, color);
            }else {
                drawTextVehicle(camera, gl, mvp, 2, wheelbase+1,
                                QString("%1L").arg(curve.howManyPathsAway,0,'f',0) + "L", 1,
                                true, color);
            }
        }
        else if (ABLine.isBtnABLineOn && (bool)ct.isContourBtnOn == false)
        {
            if(ct.isContourBtnOn)
            if (ABLine.howManyPathsAway == 0) {
                drawTextVehicle(camera, gl, mvp, 2, wheelbase+1, "0", 1,
                                true, color);
            } else if (ABLine.howManyPathsAway > 0) {
                drawTextVehicle(camera, gl, mvp, 2, wheelbase+1,
                                QString("%1R").arg(ABLine.howManyPathsAway,0,'f',0), 1,
                                true, color);
            }else {
                drawTextVehicle(camera, gl, mvp, 2, wheelbase+1,
                                QString("%1L").arg(ABLine.howManyPathsAway,0,'f',0), 1,
                                true, color);
            }
        }
    }
    */

    //if (camera.camSetDistance < -500)
    //{
    //    double theta = glm::twoPI / 20;
    //    double c = cos(theta);//precalculate the sine and cosine
    //    double s = sin(theta);

    //    double x = camera.camSetDistance * -.015;//we start at angle = 0
    //    double y = 0;

    //    gldraw.clear();
    //    gldraw.append(QVector3D(x, y, 0.0));
    //    for (int ii = 0; ii < 20; ii++)
    //    {
    //        //output vertex
    //        gldraw.append(QVector3D(x, y, 0.0));

    //        //apply the rotation matrix
    //        double t = x;
    //        x = (c * x) - (s * y);
    //        y = (s * t) + (c * y);
    //        // GL.Vertex3(x, y, 0.0);
    //    }
    //    gldraw.draw(gl,mvp,QColor::fromRgbF(0.5f, 0.5f, 1.2f, 0.25),
    //                GL_TRIANGLE_FAN, 1);

    //    gldraw.clear();

    //    for (int ii = 0; ii < 20; ii++)
    //    {
    //        //output vertex
    //        gldraw.append(QVector3D(x, y, 0.0));

    //        //apply the rotation matrix
    //        double t = x;
    //        x = (c * x) - (s * y);
    //        y = (s * t) + (c * y);
    //        // GL.Vertex3(x, y, 0.0);
    //    }
    //    gldraw.draw(gl, mvp, QColor::fromRgbF(0.5f, 1.2f, 0.2f),
    //                GL_LINE_LOOP, 2);
    //}
}

void CVehicle::AverageTheSpeed(double newSpeed) {
    avgSpeed = newSpeed * 0.75 + avgSpeed * 0.25;
}

void CVehicle::setIsHydLiftOn(bool value)
{
    if (isHydLiftOn != value) {
        isHydLiftOn = value;
        emit isHydLiftOnChanged();
    }
}

void CVehicle::setHydLiftDown(bool value)
{
    if (hydLiftDown != value) {
        hydLiftDown = value;
        emit hydLiftDownChanged();
    }
}

void CVehicle::setIsChangingDirection(bool value)
{
    if (isChangingDirection != value)
    {
        isChangingDirection = value;
        emit isChangingDirectionChanged();
    }
}

void CVehicle::setIsReverse(bool value)
{
    if (isReverse != value)
    {
        isReverse = value;
        emit isReverseChanged();
    }
}
