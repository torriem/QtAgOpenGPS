// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Main loop OpenGL stuff
//#include <QtOpenGL>
#include <assert.h>
#include "formgps.h"
#include "csection.h"
#include "cvehicle.h"
#include "cworldgrid.h"
#include "ccontour.h"
#include "cabline.h"
#include "cboundary.h"
#include "aogrenderer.h"
#include "cnmea.h"
#include "glm.h"
#include "glutils.h"
#include "qmlutil.h"

//#include <QGLWidget>
#include <QQuickView>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include "aogproperty.h"
#include "cpgn.h"

#include <assert.h>


QVector3D FormGPS::mouseClickToPan(int mouseX, int mouseY)
{
    /* returns easting and northing relative to the tractor's hitch position,
     * useful for drag to pan
     */

    QMatrix4x4 modelview;
    QMatrix4x4 projection;

    int width = qmlItem(qml_root, "openglcontrol")->property("width").toReal();
    int height = qmlItem(qml_root, "openglcontrol")->property("height").toReal();
    double shiftX = qmlItem(qml_root,"openglcontrol")->property("shiftX").toDouble();
    double shiftY = qmlItem(qml_root,"openglcontrol")->property("shiftY").toDouble();

    projection.setToIdentity();

    //to shift, translate projection here. -1,0,0 is far left, 1,0,0 is far right.
    projection.translate(shiftX,shiftY,0);

    //  Create a perspective transformation.
    projection.perspective(glm::toDegrees(fovy), width / (double)height, 1.0f, camDistanceFactor * camera.camSetDistance);
    modelview.setToIdentity();

    //camera does translations and rotations
    camera.SetWorldCam(modelview, vehicle.pivotAxlePos.easting, vehicle.pivotAxlePos.northing, camera.camHeading);
    modelview.translate(vehicle.hitchPos.easting, vehicle.hitchPos.northing, 0);
    //modelview.translate(sin(vehicle.fixHeading) * tool.hitchLength,
    //                        cos(vehicle.fixHeading) * tool.hitchLength, 0);
    if (camera.camFollowing)
        modelview.rotate(glm::toDegrees(-vehicle.fixHeading), 0.0, 0.0, 1.0);

    float x,y;
    x = mouseX;
    y = height - mouseY;

    //get point on the near plane
    QVector3D worldpoint_near = QVector3D( { x, y, 0} ).unproject(modelview,projection,QRect(0,0,width, height));
    //get point on the far plane
    QVector3D worldpoint_far = QVector3D( { x, y, 1} ).unproject(modelview, projection,QRect(0,0,width, height));
    //get direction vector from near to far
    QVector3D direction = worldpoint_far - worldpoint_near;
    //determine intercept with z=0 plane, and calculate easting and northing
    double lambda = -(worldpoint_near.z()) / direction.z();

    mouseEasting = worldpoint_near.x() + lambda * direction.x();
    mouseNorthing = worldpoint_near.y() + lambda * direction.y();

    QMatrix4x4 m;
    m.rotate(-vehicle.fixHeading, 0,0,1);

    QVector3D relative = QVector3D( { (float)mouseEasting, (float)mouseNorthing, 0 } );
    return relative;
}

QVector3D FormGPS::mouseClickToField(int mouseX, int mouseY)
{
    /* returns the field easting and northing position of a
     * mouse click
     */

    QMatrix4x4 modelview;
    QMatrix4x4 projection;

    int width = qmlItem(qml_root, "openglcontrol")->property("width").toReal();
    int height = qmlItem(qml_root, "openglcontrol")->property("height").toReal();
    double shiftX = qmlItem(qml_root,"openglcontrol")->property("shiftX").toDouble();
    double shiftY = qmlItem(qml_root,"openglcontrol")->property("shiftY").toDouble();

    projection.setToIdentity();

    //to shift, translate projection here. -1,0,0 is far left, 1,0,0 is far right.
    projection.translate(shiftX,shiftY,0);

    //  Create a perspective transformation.
    projection.perspective(glm::toDegrees(fovy), width / (double)height, 1.0f, camDistanceFactor * camera.camSetDistance);
    modelview.setToIdentity();

    //camera does translations and rotations
    camera.SetWorldCam(modelview, vehicle.pivotAxlePos.easting, vehicle.pivotAxlePos.northing, camera.camHeading);
    //modelview.translate(vehicle.pivotAxlePos.easting, vehicle.pivotAxlePos.northing, 0);
    //modelview.translate(sin(vehicle.fixHeading) * tool.hitchLength,
    //                        cos(vehicle.fixHeading) * tool.hitchLength, 0);
    //if (camera.camFollowing)
    //    modelview.rotate(glm::toDegrees(-vehicle.fixHeading), 0.0, 0.0, 1.0);

    float x,y;
    x = mouseX;
    y = height - mouseY;

    //get point on the near plane
    QVector3D worldpoint_near = QVector3D( { x, y, 0} ).unproject(modelview,projection,QRect(0,0,width, height));
    //get point on the far plane
    QVector3D worldpoint_far = QVector3D( { x, y, 1} ).unproject(modelview, projection,QRect(0,0,width, height));
    //get direction vector from near to far
    QVector3D direction = worldpoint_far - worldpoint_near;
    //determine intercept with z=0 plane, and calculate easting and northing
    double lambda = -(worldpoint_near.z()) / direction.z();

    mouseEasting = worldpoint_near.x() + lambda * direction.x();
    mouseNorthing = worldpoint_near.y() + lambda * direction.y();

    QMatrix4x4 m;
    m.rotate(-vehicle.fixHeading, 0,0,1);

    QVector3D fieldCoord = QVector3D( { (float)mouseEasting, (float)mouseNorthing, 0 } );
    return fieldCoord;
}

void FormGPS::oglMain_Paint()
{
    QOpenGLContext *glContext = QOpenGLContext::currentContext();
    QOpenGLFunctions *gl = glContext->functions();
    //int width = glContext->surface()->size().width();
    //int height = glContext->surface()->size().height();
    QMatrix4x4 projection;
    QMatrix4x4 modelview;
    QColor color;
    GLHelperTexture gldrawtex;
    GLHelperColors gldrawcolors;
    GLHelperOneColor gldraw1;

    //synchronize with the position code in the main thread
    //if (newframe)
    //    qDebug() << "start of new frame, waiting for lock at " << swFrame.elapsed();
    if (!lock.tryLockForRead())
        //if there's no new position to draw, just return so we don't
        //waste time redrawing.  Frame rate is at most gpsHz.  And if we
        //need to redraw part of the window on a resize, it will just have
        //to wait until the next position comes in.  Although if there is
        //no simulator running and no positions coming in, the GL background
        //will not update, which isn't what we want either.  Some kind of timeout?
     return;

    //if (newframe)
    //    qDebug() << "start of new frame render at " << swFrame.elapsed();

    int width = qmlItem(qml_root, "openglcontrol")->property("width").toReal();
    int height = qmlItem(qml_root, "openglcontrol")->property("height").toReal();
    double shiftX = qmlItem(qml_root,"openglcontrol")->property("shiftX").toDouble();
    double shiftY = qmlItem(qml_root,"openglcontrol")->property("shiftY").toDouble();
    //gl->glViewport(0,0,width,height);
    //qDebug() << width << height;

    /*
#ifdef GL_POINT_SPRITE
    //not compatible with OpenGL ES
    gl->glEnable(GL_POINT_SPRITE);
#endif
    */
#ifdef GL_PROGRAM_POINT_SIZE
    //not required on OpenGL ES
    gl->glEnable(GL_PROGRAM_POINT_SIZE);
#endif

    //Do stuff that was in the initialized method, since Qt uses OpenGL and may
    //have messed with the matrix stuff and other defaults

    // Set The Blending Function For Translucency
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl->glCullFace(GL_BACK);

    //gl->glDisable(GL_CULL_FACE);

    //set the camera to right distance
    SetZoom();

    //now move the "camera" to the calculated zoom settings
    //I had to move these functions here because if setZoom is called
    //from elsewhere in the GUI (say a button press), there's no GL
    //context to work with.
    projection.setToIdentity();

    //to shift, translate projection here. -1,0,0 is far left, 1,0,0 is far right.

    //warning.  Moving in the Y direction alters the way the field tilts in 3D view.
    //would need to adjust the camera.setWorldCam stuff.
    //But can move in X direction without issue
    projection.translate(shiftX,shiftY,0);


    //  Create a perspective transformation.
    projection.perspective(glm::toDegrees(fovy), width / (double)height, 1.0f, camDistanceFactor * camera.camSetDistance);

    //oglMain rendering, Draw

    int deadCam = 0;

    gl->glEnable(GL_BLEND);
    gl->glClearColor(0.25122f, 0.258f, 0.275f, 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl->glDisable(GL_DEPTH_TEST);
    //gl->glDisable(GL_TEXTURE_2D);


    if((uint)sentenceCounter < 299)
    {
        if (isGPSPositionInitialized)
        {

            //  Clear the color and depth buffer.
            gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (isDay) gl->glClearColor(0.27f, 0.4f, 0.7f, 1.0f);
            else gl->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            modelview.setToIdentity();

            //camera does translations and rotations
            camera.SetWorldCam(modelview, vehicle.pivotAxlePos.easting, vehicle.pivotAxlePos.northing, camera.camHeading);

            //calculate the frustum planes for culling
            CalcFrustum(projection*modelview);

            QColor fieldcolor;
            if(isDay) {
                fieldcolor = fieldColorDay;
            } else {
                fieldcolor = fieldColorNight;
            }
            //draw the field ground images
            worldGrid.DrawFieldSurface(gl, projection *modelview, isTextureOn, fieldcolor, camera);

            ////if grid is on draw it
            if (isGridOn)
                worldGrid.DrawWorldGrid(gl,projection, modelview, gridZoom, QColor::fromRgbF(0,0,0));

            //OpenGL ES does not support wireframe in this way. If we want wireframe,
            //we'll have to do it with LINES
            //if (isDrawPolygons) gl->glPolygonMode(GL_FRONT, GL_LINE);

            gl->glEnable(GL_BLEND);
            //draw patches of sections

            for (int j = 0; j < triStrip.count(); j++)
            {
                //every time the section turns off and on is a new patch
                bool isDraw;

                int patches = triStrip[j].patchList.size();

                if (patches > 0)
                {
                    //initialize the steps for mipmap of triangles (skipping detail while zooming out)
                    /* Unused since we use the triangle lists directly with the buffers
                    int mipmap = 0;
                    if (camera.camSetDistance < -800) mipmap = 2;
                    if (camera.camSetDistance < -1500) mipmap = 4;
                    if (camera.camSetDistance < -2400) mipmap = 8;
                    if (camera.camSetDistance < -4800) mipmap = 16;
                    */

                    //for every new chunk of patch
                    for (QSharedPointer<PatchTriangleList> &triList: triStrip[j].patchList)
                    {
                        isDraw = false;
                        int count2 = triList->size();
                        for (int i = 1; i < count2; i += 3) //first vertice is color
                        {
                            //determine if point is in frustum or not, if < 0, its outside so abort, z always is 0
                            //x is easting, y is northing
                            if (frustum[0] * (*triList)[i].x() + frustum[1] * (*triList)[i].y() + frustum[3] <= 0)
                                continue;//right
                            if (frustum[4] * (*triList)[i].x() + frustum[5] * (*triList)[i].y() + frustum[7] <= 0)
                                continue;//left
                            if (frustum[16] * (*triList)[i].x() + frustum[17] * (*triList)[i].y() + frustum[19] <= 0)
                                continue;//bottom
                            if (frustum[20] * (*triList)[i].x() + frustum[21] * (*triList)[i].y() + frustum[23] <= 0)
                                continue;//top
                            if (frustum[8] * (*triList)[i].x() + frustum[9] * (*triList)[i].y() + frustum[11] <= 0)
                                continue;//far
                            if (frustum[12] * (*triList)[i].x() + frustum[13] * (*triList)[i].y() + frustum[15] <= 0)
                                continue;//near

                            //point is in frustum so draw the entire patch. The downside of triangle strips.
                            isDraw = true;
                            break;
                        }

                        if (isDraw)
                        {
                            color.setRgbF((*triList)[0].x(), (*triList)[0].y(), (*triList)[0].z(), 0.596 );
                            //QVector<QVector3D> vertices;
                            QOpenGLBuffer triBuffer;

                            triBuffer.create();
                            triBuffer.bind();

                            //triangle lists are now using QVector3D, so we can allocate buffers
                            //directly from list data.
                            triBuffer.allocate(triList->data() + 1, (count2-1) * sizeof(QVector3D));
                            triBuffer.release();

                            glDrawArraysColor(gl,projection*modelview,
                                              GL_TRIANGLE_STRIP, color,
                                              triBuffer,GL_FLOAT,count2-1);

                            triBuffer.destroy();
                        }
                    }
                }
            }

            // the follow up to sections patches
            int patchCount = 0;

            if (patchCounter > 0)
            {
                color = sectionColorDay;
                if (isDay) color.setAlpha(152);
                else color.setAlpha(76);

                for (int j = 0; j < triStrip.count(); j++)
                {
                    if (triStrip[j].isDrawing)
                    {
                        if (tool.isMultiColoredSections)
                        {
                            color = tool.secColors[j];
                            color.setAlpha(152);
                        }
                        patchCount = triStrip[j].patchList.count();

                       //draw the triangle in each triangle strip
                        gldraw1.clear();

                        //left side of triangle
                        QVector3D pt((vehicle.cosSectionHeading * tool.section[triStrip[j].currentStartSectionNum].positionLeft) + vehicle.toolPos.easting,
                                (vehicle.sinSectionHeading * tool.section[triStrip[j].currentStartSectionNum].positionLeft) + vehicle.toolPos.northing, 0);
                        gldraw1.append(pt);

                        //Right side of triangle
                        pt = QVector3D((vehicle.cosSectionHeading * tool.section[triStrip[j].currentEndSectionNum].positionRight) + vehicle.toolPos.easting,
                           (vehicle.sinSectionHeading * tool.section[triStrip[j].currentEndSectionNum].positionRight) + vehicle.toolPos.northing, 0);
                        gldraw1.append(pt);

                        int last = triStrip[j].patchList[patchCount -1]->count();
                        //antenna
                        gldraw1.append(QVector3D((*triStrip[j].patchList[patchCount-1])[last-2].x(), (*triStrip[j].patchList[patchCount-1])[last-2].y(),0));
                        gldraw1.append(QVector3D((*triStrip[j].patchList[patchCount-1])[last-1].x(), (*triStrip[j].patchList[patchCount-1])[last-1].y(),0));

                        gldraw1.draw(gl, projection*modelview, color, GL_TRIANGLE_STRIP, 1.0f);
                    }
                }
            }

            if (tram.displayMode != 0) tram.DrawTram(gl,projection*modelview,camera);

            //draw contour line if button on
            if (ct.isContourBtnOn)
            {
                ct.DrawContourLine(gl, projection*modelview);
            }
            else// draw the current and reference AB Lines or CurveAB Ref and line
            {
                //when switching lines, draw the ghost
                if (trk.idx > -1)
                {
                    if (trk.gArr[trk.idx].mode == (int)TrackMode::AB)
                        ABLine.DrawABLines(gl,projection * modelview, isFontOn, trk, yt, camera, gyd);
                    else
                        curve.DrawCurve(gl, projection * modelview, isFontOn, trk, yt, camera);
                }
            }

            //if (recPath.isRecordOn)
            recPath.DrawRecordedLine(gl, projection*modelview);
            recPath.DrawDubins(gl, projection*modelview);

            if (bnd.bndList.count() > 0 || bnd.isBndBeingMade == true)
            {
                //draw Boundaries
                bnd.DrawFenceLines(vehicle, mc, gl, projection*modelview);

                //draw the turnLines
                if (yt.isYouTurnBtnOn && ! ct.isContourBtnOn)
                {
                    bnd.DrawFenceLines(vehicle,mc,gl,projection*modelview);

                    color.setRgbF(0.3555f, 0.6232f, 0.20f); //TODO: not sure what color turnLines should actually be

                    for (int i = 0; i < bnd.bndList.count(); i++)
                    {
                        DrawPolygon(gl,projection*modelview,bnd.bndList[i].turnLine,(float)property_setDisplay_lineWidth,color);
                    }
                }

                //Draw headland
                if (bnd.isHeadlandOn)
                {
                    color.setRgbF(0.960f, 0.96232f, 0.30f);
                    DrawPolygon(gl,projection*modelview,bnd.bndList[0].hdLine,(float)property_setDisplay_lineWidth,color);
                }
            }

            //Direct line to flag if flag selected
            if (flagNumberPicked > 0)
            {
                gldraw1.clear();
                //TODO: implement with shader: GL.LineStipple(1, 0x0707);
                gldraw1.append(QVector3D(vehicle.pivotAxlePos.easting, vehicle.pivotAxlePos.northing, 0));
                gldraw1.append(QVector3D(flagPts[flagNumberPicked-1].easting, flagPts[flagNumberPicked-1].northing, 0));
                gldraw1.draw(gl, projection*modelview,
                             QColor::fromRgbF(0.930f, 0.72f, 0.32f),
                             GL_LINES, property_setDisplay_lineWidth);
            }

            //draw the vehicle/implement
            QMatrix4x4 mv = modelview; //push matrix
            tool.DrawTool(gl,modelview, projection,isJobStarted,vehicle, camera,tram);
            double steerangle;
            if(timerSim.isActive()) steerangle = sim.steerangleAve;
            else steerangle = mc.actualSteerAngleDegrees;
            vehicle.DrawVehicle(gl, modelview, projection, steerangle, isFirstHeadingSet,
                                QRect(0,0,width,height),camera,tool,bnd);
            modelview = mv; //pop matrix

            if (camera.camSetDistance > -150)
            {
                gldraw1.clear();
                color.setRgbF(0.98, 0.98, 0.098);
                if (trk.idx > -1)
                {
                    if (trk.gArr[trk.idx].mode == (int)TrackMode::AB)
                    {
                        gldraw1.clear();
                        gldraw1.append(QVector3D(ABLine.goalPointAB.easting, ABLine.goalPointAB.northing, 0));
                        gldraw1.draw(gl,projection*modelview,QColor::fromRgbF(0,0,0),GL_POINTS,16);
                        gldraw1.draw(gl,projection*modelview,color,GL_POINTS,10);
                    }
                    else
                    {
                        gldraw1.clear();
                        gldraw1.append(QVector3D(curve.goalPointCu.easting, curve.goalPointCu.northing, 0));
                        gldraw1.draw(gl,projection*modelview,QColor::fromRgbF(0,0,0),GL_POINTS,16);
                        gldraw1.draw(gl,projection*modelview,color,GL_POINTS,10);
                    }
                }
            }

            // 2D Ortho --------------------------
            //no need to "push" matrix since it will be regenerated next time
            projection.setToIdentity();
            //negative and positive on width, 0 at top to bottom ortho view
            projection.ortho(-(double)width / 2, (double)width / 2, (double)height, 0, -1, 1);

            //  Create the appropriate modelview matrix.
            modelview.setToIdentity();

            //lightbar, button status, compasstext, steer circle, Hyd Lift, RTK
            // alarm, etc are all done in QML based on properties in Interface.qml

            //uturn buttons drawn in QML

            if (tool.isDisplayTramControl && tram.displayMode != 0) { DrawTramMarkers(gl,projection*modelview); }

            //if this is on, vehicleInterface.isHydLiftOn is true
            if (p_239.pgn[p_239.hydLift] == 2)
            {
                vehicle.hydLiftDown = false; //vehicleInterface.hydLiftDown in QML
            }
            else
            {
                vehicle.hydLiftDown = true;
            }

            //Reverse indicator in QML
            //RTK alarm implemented in QML
            //RTK age implemented in QML
            //guidance line text implemented in QML

            gl->glFlush();

            //draw the zoom window
            if (isJobStarted)
            {
                /*TODO implement floating zoom windo
                if (threeSeconds != zoomUpdateCounter)
                {
                    zoomUpdateCounter = threeSeconds;
                    oglZoom.Refresh();
                }
                */
            }
            if (leftMouseDownOnOpenGL) MakeFlagMark(gl); //TODO: not working, fix!
        }
        else
        {
            gl->glClear(GL_COLOR_BUFFER_BIT);
        }

        //directly call section lookahead GL stuff from here
        if (! newframe) {
            //No new position, so no need to repaint the back buffer
            //and do section look-ahead
            lock.unlock();
            //qWarning() << "rendered but skipping section lookahead processing.";
            return;
        }
        oglBack_Paint();
        gl->glFlush();

    }
    else
    {
        modelview.setToIdentity();
        modelview.translate(0,0.3,-10);
        //rotate the camera down to look at fix
        //modelview.rotate(-60, 1.0, 0.0, 0.0);
        modelview.rotate(deadCam, 0.0, 1.0, 0.0);
        deadCam += 5;

        //TODO: replace with QML widget

        //draw with NoGPS texture 21
       /* color.setRgbF(1.25f, 1.25f, 1.275f, 0.75);
        gldrawtex.append( { QVector3D(2.5, 2.5, 0), QVector2D(1,0) } ); //Top Right
        gldrawtex.append( { QVector3D(-2.5, 2.5, 0), QVector2D(0,0) } ); //Top Left
        gldrawtex.append( { QVector3D(2.5, -2.5, 0), QVector2D(1,1) } ); //Bottom Right
        gldrawtex.append( { QVector3D(-2.5, -2.5, 0), QVector2D(0,1) } ); //Bottom Left

        gldrawtex.draw(gl, projection * modelview, Textures::NOGPS, GL_TRIANGLE_STRIP, true,color);
*/

        // 2D Ortho ---------------------------------------////////-------------------------------------------------

        //we don't need to save the matrix since it's regenerated every time through this method
        projection.setToIdentity();

        //negative and positive on width, 0 at top to bottom ortho view
        projection.ortho(-(double)width / 2, (double)width / 2, (double)height, 0, -1, 1);

        //  Create the appropriate modelview matrix.
        modelview.setToIdentity();

        color.setRgbF(0.98f, 0.98f, 0.70f);
        int edge = -(double)width / 2 + 10;

        drawText(gl,projection * modelview,edge,height - 240, "<-- AgIO ?",1.0,true,color);

        gl->glFlush();

        //GUI widgets have to be updated elsewhere
    }
    lock.unlock();
    if(newframe && (bool)isJobStarted) {
        //if we just had a new position and updated the back buffer then
        //proecss the section lookaheads:
        QTimer::singleShot(0,this, &FormGPS::processSectionLookahead);
        //qDebug() << "GL thread is different: " << !(QThread::currentThread() == QCoreApplication::instance()->thread());
        //qDebug() << "end of new frame render at " << swFrame.elapsed();
    }
    newframe = false;
}

/// Handles the OpenGLInitialized event of the openGLControl control.
void FormGPS::openGLControl_Initialized()
{
    //QOpenGLContext *glContext = QOpenGLContext::currentContext();

    //qmlview->resetOpenGLState();

    //Load all the textures
    //qDebug() << "initializing Open GL.";
    loadGLTextures();
    //qDebug() << "textures loaded.";
    initializeShaders();
    //qDebug() << "shaders loaded.";

    /*
    //load shaders, memory managed by parent thread, which is in this case,
    //the QML rendering thread, not the Qt main loop.
    if (!simpleColorShader) {
        simpleColorShader = new QOpenGLShaderProgram(QThread::currentThread()); //memory managed by Qt
        assert(simpleColorShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/color_vshader.vsh"));
        assert(simpleColorShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/color_fshader.fsh"));
        assert(simpleColorShader->link());
    }
    if (!texShader) {
        texShader = new QOpenGLShaderProgram(QThread::currentThread()); //memory managed by Qt
        assert(texShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/colortex_vshader.vsh"));
        assert(texShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/colortex_fshader.fsh"));
        assert(texShader->link());
    }
    if (!interpColorShader) {
        interpColorShader = new QOpenGLShaderProgram(QThread::currentThread()); //memory managed by Qt
        assert(interpColorShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/colors_vshader.vsh"));
        assert(interpColorShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/colors_fshader.fsh"));
        assert(interpColorShader->link());
    }
    */

    //now start the timer assuming no errors, otherwise the program will not stop on errors.
    //TODO:
    //tmrWatchdog.Enabled = true;
    //qmlview->resetOpenGLState();
}

void FormGPS::openGLControl_Shutdown()
{

    //qDebug() << "OpenGL shutting down... destroying buffers and shaders";
    //qDebug() << QOpenGLContext::currentContext();
    //We should have a valid OpenGL context here so we can clean up shaders and buffers
    destroyShaders();
    destroyTextures();
    //destroy any openGL buffers.
    worldGrid.destroyGLBuffers();
}

//main openGL draw function
void FormGPS::oglBack_Paint()
{
    //Because this is potentially running in another thread, we cannot
    //safely make any GUI calls to set buttons, etc.  So instead, we
    //do the GL drawing here and get the lookahead pixmap from GL here.
    //After this, this widget will emit a finished signal, where the main
    //thread can then run the second part of this function, which I've
    //split out into its own function.
    QOpenGLContext *glContext = QOpenGLContext::currentContext();
    QMatrix4x4 projection;
    QMatrix4x4 modelview;

    GLHelperOneColor gldraw;

    /* use the QML context with an offscreen surface to draw
     * the lookahead triangles
     */
    if (!backFBO ) {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        //TODO: backFBO is leaking... delete it in the destructor?
        //I think context has to be active to delete it...
        backFBO = new QOpenGLFramebufferObject(QSize(500,300),format);
    }
    QSurface *origSurface = glContext->surface();

    glContext->makeCurrent(&backSurface);
    backFBO->bind();
    glContext->functions()->glViewport(0,0,500,300);
    QOpenGLFunctions *gl = glContext->functions();

    //int width = glContext->surface()->size().width();
    //int height = glContext->surface()->size().height();

    gl->glPixelStorei(GL_PACK_ALIGNMENT, 1);

    //  Load the identity.
    projection.setToIdentity();

    //projection.perspective(6.0f,1,1,6000);
    projection.perspective(glm::toDegrees((double)0.06f), 1.666666666666f, 50.0f, 520.0f);

    gl->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
    //gl->glLoadIdentity();					// Reset The View
    modelview.setToIdentity();

    //back the camera up
    modelview.translate(0, 0, -500);

    //rotate camera so heading matched fix heading in the world
    //gl->glRotated(toDegrees(vehicle.fixHeadingSection), 0, 0, 1);
    modelview.rotate(glm::toDegrees(vehicle.toolPos.heading), 0, 0, 1);

    modelview.translate(-vehicle.toolPos.easting - sin(vehicle.toolPos.heading) * 15,
                        -vehicle.toolPos.northing - cos(vehicle.toolPos.heading) * 15,
                        0);

    //patch color
    QColor patchColor = QColor::fromRgbF(0.0f, 0.5f, 0.0f);

    //to draw or not the triangle patch
    bool isDraw;

    double pivEplus = vehicle.pivotAxlePos.easting + 50;
    double pivEminus = vehicle.pivotAxlePos.easting - 50;
    double pivNplus = vehicle.pivotAxlePos.northing + 50;
    double pivNminus = vehicle.pivotAxlePos.northing - 50;

    //draw patches j= # of sections
    for (int j = 0; j < triStrip.count(); j++)
    {
        //every time the section turns off and on is a new patch
        int patchCount = triStrip[j].patchList.size();

        if (patchCount > 0)
        {
            //for every new chunk of patch
            for (QSharedPointer<QVector<QVector3D>> &triList: triStrip[j].patchList)
            {
                isDraw = false;
                int count2 = triList->size();
                for (int i = 1; i < count2; i+=3)
                {
                    //determine if point is in frustum or not
                    if ((*triList)[i].x() > pivEplus)
                        continue;
                    if ((*triList)[i].x() < pivEminus)
                        continue;
                    if ((*triList)[i].y() > pivNplus)
                        continue;
                    if ((*triList)[i].y() < pivNminus)
                        continue;

                    //point is in frustum so draw the entire patch
                    isDraw = true;
                    break;
                }

                if (isDraw)
                {
                    QOpenGLBuffer triBuffer;

                    triBuffer.create();
                    triBuffer.bind();

                    //triangle lists are now using QVector3D, so we can allocate buffers
                    //directly from list data.

                    //first vertice is color, so we should skip it
                    triBuffer.allocate(triList->data() + 1, (count2-1) * sizeof(QVector3D));
                    //triBuffer.allocate(triList->data(), count2 * sizeof(QVector3D));
                    triBuffer.release();

                    //draw the triangles in each triangle strip
                    glDrawArraysColor(gl,projection*modelview,
                                      GL_TRIANGLE_STRIP, patchColor,
                                      triBuffer,GL_FLOAT,count2-1);

                    triBuffer.destroy();
                }
            }
        }
    }

    //draw tool bar for debugging
    //gldraw.clear();
    //gldraw.append(QVector3D(tool.section[0].leftPoint.easting, tool.section[0].leftPoint.northing,0.5));
    //gldraw.append(QVector3D(tool.section[tool.numOfSections-1].rightPoint.easting, tool.section[tool.numOfSections-1].rightPoint.northing,0.5));
    //gldraw.draw(gl,projection*modelview,QColor::fromRgb(255,0,0),GL_LINE_STRIP,1);

    //draw 245 green for the tram tracks

    if (tram.displayMode !=0 && tram.displayMode !=0 && (trk.idx > -1))
    {
        if ((tram.displayMode == 1 || tram.displayMode == 2))
        {
            for (int i = 0; i < tram.tramList.count(); i++)
            {
                gldraw.clear();
                for (int h = 0; h < tram.tramList[i]->count(); h++)
                    gldraw.append(QVector3D((*tram.tramList[i])[h].easting, (*tram.tramList[i])[h].northing, 0));
                gldraw.draw(gl,projection*modelview,QColor::fromRgb(0,245,0),GL_LINE_STRIP,8);
            }
        }

        if (tram.displayMode == 1 || tram.displayMode == 3)
        {
            gldraw.clear();
            for (int h = 0; h < tram.tramBndOuterArr.count(); h++)
                gldraw.append(QVector3D(tram.tramBndOuterArr[h].easting, tram.tramBndOuterArr[h].northing, 0));
            for (int h = 0; h < tram.tramBndInnerArr.count(); h++)
                gldraw.append(QVector3D(tram.tramBndInnerArr[h].easting, tram.tramBndInnerArr[h].northing, 0));
            gldraw.draw(gl,projection*modelview,QColor::fromRgb(0,245,0),GL_LINE_STRIP,8);
        }
    }

    //draw 240 green for boundary
    if (bnd.bndList.count() > 0)
    {
        ////draw the bnd line
        if (bnd.bndList[0].fenceLine.count() > 3)
        {
            DrawPolygon(gl,projection*modelview,bnd.bndList[0].fenceLine,3,QColor::fromRgb(0,240,0));
        }


        //draw 250 green for the headland
        if (bnd.isHeadlandOn && bnd.isSectionControlledByHeadland)
        {
            DrawPolygon(gl,projection*modelview,bnd.bndList[0].hdLine,3,QColor::fromRgb(0,250,0));
        }
    }

    //finish it up - we need to read the ram of video card
    gl->glFlush();

    //read the whole block of pixels up to max lookahead, one read only
    //we'll use Qt's QImage function to grab it.
    grnPix = backFBO->toImage().mirrored().convertToFormat(QImage::Format_RGBX8888);
    //qDebug() << grnPix.size();
    //QImage temp = grnPix.copy(tool.rpXPosition, 250, tool.rpWidth, 290 /*(int)rpHeight*/);
    //TODO: is thisn right?
    QImage temp = grnPix.copy(tool.rpXPosition, 0, tool.rpWidth, 290 /*(int)rpHeight*/);
    temp.setPixelColor(0,0,QColor::fromRgb(255,128,0));
    grnPix = temp; //only show clipped image
    memcpy(grnPixels, temp.constBits(), temp.size().width() * temp.size().height() * 4);
    //grnPix = temp;

    //first channel
    if (worldGrid.numRateChannels > 0)
    {
        /*
        GL.Enable(EnableCap.Texture2D);

        GL.BindTexture(TextureTarget.Texture2D, texture[(int)textures.RateMap1]);
        GL.Begin(PrimitiveType.TriangleStrip);
        GL.Color3(1.0f, 1.0f, 1.0f);
        GL.TexCoord2(0, 0);
        GL.Vertex3(worldGrid.eastingMinRate, worldGrid.northingMaxRate, 0.10);
        GL.TexCoord2(1, 0.0);
        GL.Vertex3(worldGrid.eastingMaxRate, worldGrid.northingMaxRate, 0.10);
        GL.TexCoord2(0.0, 1);
        GL.Vertex3(worldGrid.eastingMinRate, worldGrid.northingMinRate, 0.10);
        GL.TexCoord2(1, 1);
        GL.Vertex3(worldGrid.eastingMaxRate, worldGrid.northingMinRate, 0.0);
        GL.End();

        GL.Flush();

        //read the whole block of pixels up to max lookahead, one read only
        GL.ReadPixels(250, 1, 1, 1, OpenTK.Graphics.OpenGL.PixelFormat.Red, PixelType.UnsignedByte, rateRed);
        */
    }


    //The remaining code from the original method in the C# code is
    //broken out into a callback in formgps.cpp called
    //processSectionLookahead().

    glContext->functions()->glFlush();

    //restore QML's context
    backFBO->bindDefault();
    glContext->doneCurrent();
    glContext->makeCurrent(origSurface);
    //resetOpenGLState();
}

//Draw section OpenGL window, not visible
void FormGPS::openGLControlBack_Initialized()
{
}

void FormGPS::MakeFlagMark(QOpenGLFunctions *gl)
{
    leftMouseDownOnOpenGL = false;
    uchar data1[768];
    memset(data1,0,768);

    qDebug() << "mouse down at " << mouseX << ", " << mouseY;
    //scan the center of click and a set of square points around
    gl->glReadPixels(mouseX - 8, mouseY - 8, 16, 16, GL_RGB, GL_UNSIGNED_BYTE, data1);

    //made it here so no flag found
    flagNumberPicked = 0;

    for (int ctr = 0; ctr < 768; ctr += 3)
    {
        if ((data1[ctr] == 255) || (data1[ctr + 1] == 255))
        {
            flagNumberPicked = data1[ctr + 2];
            qDebug() << flagNumberPicked;
            break;
        }
    }

    /*TODO: popup flag menu*/
    //have to set a flag for the main loop

}

void FormGPS::DrawTramMarkers(QOpenGLFunctions *gl, QMatrix4x4 mvp)
{
    //int sizer = 60;
    int center = -50 ;
    int bottomSide = 100;

    GLHelperTexture gltex;

    QColor dot_color;

    if (((tram.controlByte) & 2) == 2) dot_color = QColor::fromRgbF(0.29f, 0.990f, 0.290f, 0.983f);
    else dot_color = QColor::fromRgbF(0.9f, 0.0f, 0.0f, 0.53f);

    if ((bool)tram.isLeftManualOn)
    {
        if (isFlashOnOff) dot_color = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.993f);
        else dot_color = QColor::fromRgbF(0.99f, 0.990f, 0.0f, 0.993f);
    }

    gltex.append( { QVector3D(center - 32, bottomSide - 32, 0), QVector2D(0,0) } );
    gltex.append( { QVector3D(center + 32, bottomSide - 32, 0), QVector2D(1, 0) } );
    gltex.append( { QVector3D(center + 32, bottomSide + 32, 0), QVector2D(1, 1) } );
    gltex.append( { QVector3D(center - 32, bottomSide + 32, 0), QVector2D(0, 1) });

    gltex.draw(gl,mvp,Textures::TRAMDOT,GL_QUADS,true,dot_color);

    if (((tram.controlByte) & 1) == 1) dot_color = QColor::fromRgbF(0.29f, 0.990f, 0.290f, 0.983f);
    else dot_color = QColor::fromRgbF(0.9f, 0.0f, 0.0f, 0.53f);

    if ((bool)tram.isRightManualOn)
    {
        if (isFlashOnOff) dot_color = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.993f);
        else dot_color = QColor::fromRgbF(0.99f, 0.990f, 0.0f, 0.993f);
    }

    center += 100;

    gltex.clear();

    gltex.append( { QVector3D(center - 32, bottomSide - 32, 0), QVector2D(0, 0) } );
    gltex.append( { QVector3D(center + 32, bottomSide - 32, 0), QVector2D(1, 0) } );
    gltex.append( { QVector3D(center + 32, bottomSide + 32, 0), QVector2D(1, 1) } );
    gltex.append( { QVector3D(center - 32, bottomSide + 32, 0), QVector2D(0, 1) } );

    gltex.draw(gl,mvp,Textures::TRAMDOT,GL_QUADS,true,dot_color);

}

void FormGPS::DrawFlags(QOpenGLFunctions *gl, QMatrix4x4 mvp)
{
    GLHelperOneColor gldraw;

    int flagCnt = flagPts.count();
    for (int f = 0; f < flagCnt; f++)
    {
        QColor color;
        QString flagColor = "&";

        if (flagPts[f].color == 0) {
            color = QColor::fromRgb(255, 0, flagPts[f].ID);
        }
        if (flagPts[f].color == 1) {
            color = QColor::fromRgb(0, 255, flagPts[f].ID);
            flagColor = "|";
        }
        if (flagPts[f].color == 2) {
            color = QColor::fromRgb(255, 255, flagPts[f].ID);
            flagColor = "~";
        }

        gldraw.append(QVector3D(flagPts[f].easting, flagPts[f].northing, 0));
        gldraw.draw(gl, mvp, color, GL_POINTS, 8.0f);
        flagColor += flagPts[f].notes;

        drawText3D(camera, gl, mvp, flagPts[f].easting, flagPts[f].northing, flagColor,1,true,color);
    }

    if (flagNumberPicked != 0)
    {
        ////draw the box around flag
        gldraw.clear();

        double offSet = (camera.zoomValue * camera.zoomValue * 0.01);
        gl->glLineWidth(4.0f);
        gldraw.append(QVector3D(flagPts[flagNumberPicked - 1].easting, flagPts[flagNumberPicked - 1].northing + offSet, 0));
        gldraw.append(QVector3D(flagPts[flagNumberPicked - 1].easting - offSet, flagPts[flagNumberPicked - 1].northing, 0));
        gldraw.append(QVector3D(flagPts[flagNumberPicked - 1].easting, flagPts[flagNumberPicked - 1].northing - offSet, 0));
        gldraw.append(QVector3D(flagPts[flagNumberPicked - 1].easting + offSet, flagPts[flagNumberPicked - 1].northing, 0));
        gldraw.append(QVector3D(flagPts[flagNumberPicked - 1].easting, flagPts[flagNumberPicked - 1].northing + offSet, 0));
        gldraw.draw(gl, mvp, QColor::fromRgbF(0.980f, 0.0f, 0.980f),
                    GL_LINE_STRIP, 4.0);
    }
}

void FormGPS::CalcFrustum(const QMatrix4x4 &mvp)
{
    //const float *clip = mvp.constData(); //column major order

    // Extract the RIGHT clipping plane
    frustum[0] = mvp(3,0) - mvp(0,0);
    frustum[1] = mvp(3,1) - mvp(0,1);
    frustum[2] = mvp(3,2) - mvp(0,2);
    frustum[3] = mvp(3,3) - mvp(0,3);
    //frustum[0] = clip[3] - clip[0];
    //frustum[1] = clip[7] - clip[4];
    //frustum[2] = clip[11] - clip[8];
    //frustum[3] = clip[15] - clip[12];

    // Extract the LEFT clipping plane
    frustum[4] = mvp(0,0) + mvp(3,0);
    frustum[5] = mvp(0,1) + mvp(3,1);
    frustum[6] = mvp(0,2) + mvp(3,2);
    frustum[7] = mvp(0,3) + mvp(3,3);
    //frustum[4] = clip[3] + clip[0];
    //frustum[5] = clip[7] + clip[4];
    //frustum[6] = clip[11] + clip[8];
    //frustum[7] = clip[15] + clip[12];

    // Extract the FAR clipping plane
    frustum[8] = mvp(3,0) - mvp(2,0);
    frustum[9] = mvp(3,1) - mvp(2,1);
    frustum[10] = mvp(3,2) - mvp(2,2);
    frustum[11] = mvp(3,3) - mvp(2,3);
    //frustum[8] = clip[3] - clip[2];
    //frustum[9] = clip[7] - clip[6];
    //frustum[10] = clip[11] - clip[10];
    //frustum[11] = clip[15] - clip[14];

    // Extract the NEAR clipping plane.  This is last on purpose (see pointinfrustum() for reason)
    frustum[12] = mvp(2,0) + mvp(3,0);
    frustum[13] = mvp(2,1) + mvp(3,1);
    frustum[14] = mvp(2,2) + mvp(3,2);
    frustum[15] = mvp(2,3) + mvp(3,3);
    //frustum[12] = clip[3] + clip[2];
    //frustum[13] = clip[7] + clip[6];
    //frustum[14] = clip[11] + clip[10];
    //frustum[15] = clip[15] + clip[14];

    // Extract the BOTTOM clipping plane
    frustum[16] = mvp(1,0) + mvp(3,0);
    frustum[17] = mvp(1,1) + mvp(3,1);
    frustum[18] = mvp(1,2) + mvp(3,2);
    frustum[19] = mvp(1,3) + mvp(3,3);
    //frustum[16] = clip[3] + clip[1];
    //frustum[17] = clip[7] + clip[5];
    //frustum[18] = clip[11] + clip[9];
    //frustum[19] = clip[15] + clip[13];

    // Extract the TOP clipping plane
    frustum[20] = mvp(3,0) - mvp(1,0);
    frustum[21] = mvp(3,1) - mvp(1,1);
    frustum[22] = mvp(3,2) - mvp(1,2);
    frustum[23] = mvp(3,3) - mvp(1,3);
    //frustum[20] = clip[3] - clip[1];
    //frustum[21] = clip[7] - clip[5];
    //frustum[22] = clip[11] - clip[9];
    //frustum[23] = clip[15] - clip[13];
}

//take the distance from object and convert to camera data
//TODO, move Projection matrix stuff into here when OpenGL ES migration is complete
void FormGPS::SetZoom()
{
    //match grid to cam distance and redo perspective
    if (camera.camSetDistance <= -20000) gridZoom = 2000;
    if (camera.camSetDistance >= -20000 && camera.camSetDistance < -10000) gridZoom =   2012;
    if (camera.camSetDistance >= -10000 && camera.camSetDistance < -5000) gridZoom =    1006;
    if (camera.camSetDistance >= -5000 && camera.camSetDistance < -2000) gridZoom =     503;
    if (camera.camSetDistance >= -2000 && camera.camSetDistance < -1000) gridZoom =     201.2;
    if (camera.camSetDistance >= -1000 && camera.camSetDistance < -500) gridZoom =      100.6;
    if (camera.camSetDistance >= -500 && camera.camSetDistance < -250) gridZoom =       50.3;
    if (camera.camSetDistance >= -250 && camera.camSetDistance < -150) gridZoom =       25.15;
    if (camera.camSetDistance >= -150 && camera.camSetDistance < -50) gridZoom =         10.06;
    if (camera.camSetDistance >= -50 && camera.camSetDistance < -1) gridZoom = 5.03;
    //1.216 2.532


}

void FormGPS::loadGLTextures()
{

    initializeTextures();
}

//determine mins maxs of patches and whole field.
void FormGPS::calculateMinMax()
{

    minFieldX = 9999999; minFieldY = 9999999;
    maxFieldX = -9999999; maxFieldY = -9999999;


    //min max of the boundary
    //min max of the boundary
    if (bnd.bndList.count() > 0)
    {
        int bndCnt = bnd.bndList[0].fenceLine.count();
        for (int i = 0; i < bndCnt; i++)
        {
            double x = bnd.bndList[0].fenceLine[i].easting;
            double y = bnd.bndList[0].fenceLine[i].northing;

            //also tally the max/min of field x and z
            if (minFieldX > x) minFieldX = x;
            if (maxFieldX < x) maxFieldX = x;
            if (minFieldY > y) minFieldY = y;
            if (maxFieldY < y) maxFieldY = y;
        }

    }
    else
    {
        //draw patches j= # of sections
        for (int j = 0; j < triStrip.count(); j++)
        {
            //every time the section turns off and on is a new patch
            int patchCount = triStrip[j].patchList.count();

            if (patchCount > 0)
            {
                //for every new chunk of patch
                for (QSharedPointer<PatchTriangleList> &triList: triStrip[j].patchList)
                {
                    int count2 = triList->count();
                    for (int i = 0; i < count2; i += 3)
                    {
                        double x = (*triList)[i].x();
                        double y = (*triList)[i].y();

                        //also tally the max/min of field x and z
                        if (minFieldX > x) minFieldX = x;
                        if (maxFieldX < x) maxFieldX = x;
                        if (minFieldY > y) minFieldY = y;
                        if (maxFieldY < y) maxFieldY = y;
                    }
                }
            }
        }
    }


    if (maxFieldX == -9999999 || minFieldX == 9999999 || maxFieldY == -9999999 || minFieldY == 9999999)
    {
        maxFieldX = 0; minFieldX = 0; maxFieldY = 0; minFieldY = 0;
    }
    else
    {
        //the largest distancew across field
        double dist = fabs(minFieldX - maxFieldX);
        double dist2 = fabs(minFieldY - maxFieldY);

        if (dist > dist2) maxFieldDistance = (dist);
        else maxFieldDistance = (dist2);

        if (maxFieldDistance < 100) maxFieldDistance = 100;
        if (maxFieldDistance > 19900) maxFieldDistance = 19900;
        //lblMax.Text = ((int)maxFieldDistance).ToString();

        fieldCenterX = (maxFieldX + minFieldX) / 2.0;
        fieldCenterY = (maxFieldY + minFieldY) / 2.0;
    }

    headland_form.setFieldInfo(maxFieldDistance,fieldCenterX,fieldCenterY);
    headache_form.setFieldInfo(maxFieldDistance,fieldCenterX,fieldCenterY);
}
