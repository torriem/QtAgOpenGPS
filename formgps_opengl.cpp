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

#include <QGLWidget>
#include <QQuickView>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include "aogsettings.h"

#include <assert.h>

void FormGPS::openGLControl_Draw()
{
    USE_SETTINGS;

    int tool_numOfSections = SETTINGS_TOOL_NUMSECTIONS;
    int tool_numSuperSection = SETTINGS_TOOL_NUMSECTIONS + 1;

    QOpenGLContext *glContext = QOpenGLContext::currentContext();
    QOpenGLFunctions *gl = glContext->functions();
    //int width = glContext->surface()->size().width();
    //int height = glContext->surface()->size().height();
    QMatrix4x4 projection;
    QMatrix4x4 modelview;
    GLHelperOneColor gldraw1;

    int width = qmlItem(qml_root, "openglcontrol")->property("width").toReal();
    int height = qmlItem(qml_root, "openglcontrol")->property("height").toReal();
    gl->glViewport(0,0,width,height);
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

    //  Create a perspective transformation.
    projection.perspective(glm::toDegrees(fovy), width / (double)height, 1.0f, camDistanceFactor * camera.camSetDistance);

    //oglMain rendering, Draw

    int deadCam = 0;

    if(sentenceCounter > 99)
    {
        gl->glEnable(GL_BLEND);

        gl->glClearColor(0.25122f, 0.258f, 0.275f, 1.0f);

        gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        modelview.setToIdentity();

        //position the camera
        //back the camera up
        camera.camSetDistance = -40;
        SetZoom();

        modelview.translate(0.0, 0.0, -20);
        //rotate the camera down to look at fix
        modelview.rotate(-60, 1.0, 0.0, 0.0);

        camera.camHeading = 0;

        deadCam++;
        modelview.rotate(deadCam/3, 0.0, 0.0, 1.0);
        ////draw the guide
        GLHelperColors gldrawcolors;
        ColorVertex cv;

        cv.color = QVector4D(0.98f, 0.0f, 0.0f, 1.0f);
        cv.vertex = QVector3D(0.0f, -1.0f, 0.0f);
        gldrawcolors.append(cv);
        cv.color = QVector4D(0.0f, 0.98f, 0.0f, 1.0f);
        cv.vertex = QVector3D(-1.0f, 1.0f, 0.0f);
        gldrawcolors.append(cv);
        cv.color = QVector4D(0.98f, 0.98f, 0.0f, 1.0f);
        cv.vertex = QVector3D(1.0f, -0.0f, 0.0f);
        gldrawcolors.append(cv);

        gldrawcolors.draw(gl, projection * modelview, GL_TRIANGLES, 1.0); // Done Drawing Reticle

        modelview.rotate(deadCam + 90, 0.0, 0.0, 1.0);
        drawText3D(camera, gl, projection*modelview, 0,0, "  I'm Lost  ", 1, true, QColor::fromRgbF(0.98f, 0.98f, 0.0f));

        modelview.rotate(deadCam + 180, 0.0, 0.0, 1.0);
        drawText3D(camera, gl, projection*modelview,
                   0,0, "   No GPS   ", 1, true, QColor::fromRgbF(0.98f, 0.98f, 0.70f));

        // 2D Ortho ---------------------------------------////////-------------------------------------------------

        //we don't need to save the matrix since it's regenerated every time through this method
        projection.setToIdentity();

        //negative and positive on width, 0 at top to bottom ortho view
        projection.ortho(-(double)width / 2, (double)width / 2, (double)height, 0, -1, 1);

        //  Create the appropriate modelview matrix.
        modelview.setToIdentity();

        /*
        //TODO: implement serial port connections
        QColor color = QColor::fromRgbF(0.98f, 0.98f, 0.70f);

        int edge = -width / 2 + 10;
        int line = 20;

        drawText(gl, projection*modelview, edge, line, "NMEA: " + recvSentenceSettings, 1, true, color);
        line += 30;
        if (sp.isOpen)
        {
            drawText(gl, projection*modelview, edge, line, "GPS Port: Connected", 1, true, color);
        }
        else
        {
            drawText(gl, projection*modelview, edge, line, "GPS Port: Not Connected", 1, true, color);
        }

        line += 30;

        if (spAutoSteer.IsOpen)
        {
            drawText(gl, projection*modelview, edge, line, "AutoSteer Port: Connected", 1, true, color);
        }
        else
        {
            drawText(gl, projection*modelview, edge, line, "AutoSteer Port: Not Connected", 1, true, color);
        }
        line += 30;
        if (spMachine.IsOpen)
        {
            drawText(gl, projection*modelview, edge, line, "Machine Port: Connected", 1, true, color);
        }
        else
        {
            drawText(gl, projection*modelview, edge, line, "Machine Port: Not Connected", 1, true, color);
        }
        line += 30;
        if (Properties.Settings.Default.setUDP_isOn)
        {
            drawText(gl, projection*modelview, edge, line, "UDP: Counter is " + pbarUDP.ToString(), 1, true, color);
        }
        else
        {
            drawText(gl, projection*modelview, edge, line, "UDP: Off", 1, true, color);
        }
        line += 30;
        */

        gl->glFlush();

        //GUI widgets have to be updated elsewhere
    }
    else
    {
        if (isGPSPositionInitialized)
        {

            //  Clear the color and depth buffer.
            gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (isDay) gl->glClearColor(0.27f, 0.4f, 0.7f, 1.0f);
            else gl->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            modelview.setToIdentity();

            //camera does translations and rotations
            camera.SetWorldCam(modelview, vehicle.pivotAxlePos.easting + offX, vehicle.pivotAxlePos.northing + offY, camera.camHeading);

            //calculate the frustum planes for culling
            calcFrustum(projection*modelview);

            QColor fieldcolor;
            if(SETTINGS_DISPLAY_DAYMODE) {
                fieldcolor = parseColor(SETTINGS_DISPLAY_FIELDCOLORDAY);
            } else {
                fieldcolor = parseColor(SETTINGS_DISPLAY_FIELDCOLORNIGHT);
            }
            //draw the field ground images
            worldGrid.DrawFieldSurface(gl, projection *modelview, fieldcolor);

            ////Draw the world grid based on camera position
            gl->glDisable(GL_DEPTH_TEST);
            gl->glDisable(GL_TEXTURE_2D);

            ////if grid is on draw it
            if (SETTINGS_DISPLAY_SHOWGRID) worldGrid.DrawWorldGrid(gl,projection*modelview,gridZoom, QColor::fromRgbF(0,0,0));

            //turn on blend for paths
            gl->glEnable(GL_BLEND);

            QColor sectionColor;
            if(SETTINGS_DISPLAY_DAYMODE) {
                sectionColor = parseColor(SETTINGS_DISPLAY_SECTIONSCOLORDAY);
                sectionColor.setAlpha(152*0.52);
            } else {
                sectionColor = parseColor(SETTINGS_DISPLAY_SECTIONSCOLORNIGHT);
                sectionColor.setAlpha(152*0.52);
            }

            //OpenGL ES does not support wireframe in this way. If we want wireframe,
            //we'll have to do it with LINES
            //if (isDrawPolygons) gl->glPolygonMode(GL_FRONT, GL_LINE);

            //draw patches of sections
            for (int j = 0; j < tool_numSuperSection; j++)
            {
                //every time the section turns off and on is a new patch

                //check if in frustum or not
                bool isDraw;
                int patches = tool.section[j].patchList.size();

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

                    //foreach is a Qt macro that iterates over Qt containers
                    foreach (QSharedPointer<TriangleList> triList, tool.section[j].patchList)
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
                            //QVector<QVector3D> vertices;
                            QOpenGLBuffer triBuffer;

                            triBuffer.create();
                            triBuffer.bind();

                            //triangle lists are now using QVector3D, so we can allocate buffers
                            //directly from list data.
                            triBuffer.allocate(triList->data() + 1, (count2-1) * sizeof(QVector3D));
                            triBuffer.release();

                            glDrawArraysColor(gl,projection*modelview,
                                              GL_TRIANGLE_STRIP, sectionColor,
                                              triBuffer,GL_FLOAT,count2-1);

                            triBuffer.destroy();
                        }
                    }
                }
            }

            // the follow up to sections patches
            int patchCount = 0;

            if (autoBtnState == btnStates::Auto || manualBtnState == btnStates::On)
            {
                //section patch color
                if (tool.section[tool_numOfSections].isSectionOn && tool.section[tool_numOfSections].patchList.count() > 0)
                {
                    patchCount = tool.section[tool_numOfSections].patchList.count();

                    //draw the triangle in each triangle strip
                    gldraw1.clear();

                    //left side of triangle
                    QVector3D pt((vehicle.cosSectionHeading * tool.section[tool_numOfSections].positionLeft) + vehicle.toolPos.easting,
                            (vehicle.sinSectionHeading * tool.section[tool_numOfSections].positionLeft) + vehicle.toolPos.northing, 0);
                    gldraw1.append(pt);

                    //TODO: label3.Text = pt.northing.ToString();

                    //Right side of triangle
                    pt = QVector3D((vehicle.cosSectionHeading * tool.section[tool_numOfSections].positionRight) + vehicle.toolPos.easting,
                       (vehicle.sinSectionHeading * tool.section[tool_numOfSections].positionRight) + vehicle.toolPos.northing, 0);
                    gldraw1.append(pt);

                    int last = tool.section[tool_numOfSections].patchList[patchCount - 1]->count();
                    //antenna
                    gldraw1.append((*tool.section[tool_numOfSections].patchList[patchCount - 1])[last - 2]);
                    gldraw1.append((*tool.section[tool_numOfSections].patchList[patchCount - 1])[last - 1]);
                    //TODO: label4.Text = (*tool.section[tool_numOfSections].patchList[patchCount - 1])[last - 2].y().ToString();

                    gldraw1.draw(gl, projection*modelview, sectionColor, GL_TRIANGLE_STRIP, 1.0f);

                }
                else
                {
                    for (int j = 0; j < tool_numSuperSection; j++)
                    {
                        if (tool.section[j].isSectionOn && tool.section[j].patchList.count() > 0)
                        {
                            gldraw1.clear();
                            patchCount = tool.section[j].patchList.count();

                            //draw the triangle in each triangle strip
                            //left side of triangle
                            QVector3D pt((vehicle.cosSectionHeading * tool.section[j].positionLeft) + vehicle.toolPos.easting,
                                    (vehicle.sinSectionHeading * tool.section[j].positionLeft) + vehicle.toolPos.northing, 0);
                            gldraw1.append(pt);
                            //TODO: label3.Text = pt.northing.ToString();

                            //Right side of triangle
                            pt = QVector3D((vehicle.cosSectionHeading * tool.section[j].positionRight) + vehicle.toolPos.easting,
                               (vehicle.sinSectionHeading * tool.section[j].positionRight) + vehicle.toolPos.northing, 0);
                            gldraw1.append(pt);

                            int last = tool.section[j].patchList[patchCount - 1]->count();
                            //antenna
                            gldraw1.append((*tool.section[j].patchList[patchCount - 1])[last - 2]);
                            gldraw1.append((*tool.section[j].patchList[patchCount - 1])[last - 1]);
                            //TODO: label4.Text = (*tool.section[j].patchList[patchCount - 1])[last - 2].y().ToString();

                            gldraw1.draw(gl,projection*modelview, sectionColor, GL_TRIANGLE_STRIP, 1.0f);
                        }
                    }
                }
            }

            //draw contour line if button on
            if (ct.isContourBtnOn)
            {
                ct.DrawContourLine(gl, projection*modelview);
            }
            else// draw the current and reference AB Lines or CurveAB Ref and line
            {
                if (ABLine.isABLineSet | ABLine.isABLineBeingSet) ABLine.DrawABLines(gl, projection*modelview, yt, tram, camera);
                if (curve.isBtnCurveOn) curve.DrawCurve(gl, projection*modelview, vehicle, yt, tram, camera);
            }

            //if (recPath.isRecordOn)
            recPath.DrawRecordedLine(gl, projection*modelview);
            recPath.DrawDubins(gl, projection*modelview);

            //draw Boundaries
            bnd.drawBoundaryLines(vehicle, gl, projection*modelview);

            //draw the turnLines
            if (yt.isYouTurnBtnOn)
            {
                if (!ABLine.isEditing && !curve.isEditing && !ct.isContourBtnOn)
                {
                    turn.drawTurnLines(bnd, gl, projection*modelview);
                }
            }
            else if (!yt.isYouTurnBtnOn && SETTINGS_DISPLAY_UTURNALWAYSON)
            {
                if (!ABLine.isEditing && !curve.isEditing && !ct.isContourBtnOn)
                {
                    turn.drawTurnLines(bnd, gl, projection*modelview);
                }
            }

            if (mc.isOutOfBounds) gf.drawGeoFenceLines(bnd, gl, projection*modelview);

            if (hd.isOn) hd.drawHeadLines(gl, projection*modelview, SETTINGS_DISPLAY_LINEWIDTH);

            if (flagPts.size() > 0) drawFlags(gl, projection*modelview);


            if (flagNumberPicked > 0)
            {
                gl->glLineWidth(SETTINGS_DISPLAY_LINEWIDTH);
                gldraw1.clear();
                //TODO: implement with shader: GL.LineStipple(1, 0x0707);
                gldraw1.append(QVector3D(vehicle.pivotAxlePos.easting, vehicle.pivotAxlePos.northing, 0));
                gldraw1.append(QVector3D(flagPts[flagNumberPicked-1].easting, flagPts[flagNumberPicked-1].northing, 0));
                gldraw1.draw(gl, projection*modelview,
                             QColor::fromRgbF(0.930f, 0.72f, 0.32f),
                             GL_LINES, SETTINGS_DISPLAY_LINEWIDTH);
            }


            //draw the vehicle/implement
            tool.DrawTool(vehicle, camera, gl, modelview, projection);
            vehicle.DrawVehicle(gl, modelview, projection, camera, bnd, hd, ct, curve, ABLine);

            // 2D Ortho --------------------------
            //no need to "push" matrix since it will be regenerated next time
            projection.setToIdentity();
            //negative and positive on width, 0 at top to bottom ortho view
            projection.ortho(-(double)width / 2, (double)width / 2, (double)height, 0, -1, 1);

            //  Create the appropriate modelview matrix.
            modelview.setToIdentity();

            if(SETTINGS_DISPLAY_SKYON) drawSky(gl, projection*modelview, width, height);

            if(SETTINGS_DISPLAY_LIGHTBARON) {
                drawRollBar(gl, modelview, projection);
                drawLightBarText(gl, projection*modelview, width, height);
            }

            if (bnd.bndArr.size() > 0 && yt.isYouTurnBtnOn) drawUTurnBtn(gl, projection*modelview);

            //Manual UTurn buttons are now in QML and are manipulated
            //in tmrWatchdog_timeout()

            if (SETTINGS_DISPLAY_COMPASS) drawCompass(gl, modelview, projection, width - 400);

            drawCompassText(gl, projection*modelview, width - 400);

            if (SETTINGS_DISPLAY_SPEEDO) drawSpeedo(gl, modelview, projection, width, height);

            if (SETTINGS_VEHICLE_ISHYDLIFTON) drawLiftIndicator(gl, modelview, projection, width, height);

            if (SETTINGS_GPS_EXPECTRTK and pn.fixQuality != 4)
            {
                drawText(gl, projection*modelview, -width / 4, 150, "Lost RTK",
                         2.0, true, QColor::fromRgbF(0.9752f, 0.52f, 0.0f));

            }

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
            if (leftMouseDownOnOpenGL) makeFlagMark(gl); //TODO: not working, fix!
        }
        else
        {
            gl->glClear(GL_COLOR_BUFFER_BIT);
        }

        //qmlview->resetOpenGLState();

        //directly call section lookahead GL stuff from here
        openGLControlBack_Draw();
        gl->glFlush();
    }
}

/// Handles the OpenGLInitialized event of the openGLControl control.
void FormGPS::openGLControl_Initialized()
{
    //QOpenGLContext *glContext = QOpenGLContext::currentContext();

    //qmlview->resetOpenGLState();

    //Load all the textures
    qDebug() << "initializing Open GL.";
    loadGLTextures();
    qDebug() << "textures loaded.";
    initializeShaders();
    qDebug() << "shaders loaded.";

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

    qDebug() << "OpenGL shutting down... destroying buffers and shaders";
    qDebug() << QOpenGLContext::currentContext();
    //We should have a valid OpenGL context here so we can clean up shaders and buffers
    destroyShaders();
    destroyTextures();
    //destroy any openGL buffers.
    worldGrid.destroyGLBuffers();
}

//main openGL draw function
void FormGPS::openGLControlBack_Draw()
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

    USE_SETTINGS;

    int tool_numSuperSection = SETTINGS_TOOL_NUMSECTIONS + 1;

    /* use the QML context with an offscreen surface to draw
     * the lookahead triangles
     */
    if (!backFBO ) {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        //TODO: backFBO is leaking... delete it in the destructor?
        //I think context has to be active to delete it...
        backFBO = new QOpenGLFramebufferObject(QSize(500,500),format);
    }
    QSurface *origSurface = glContext->surface();

    glContext->makeCurrent(&backSurface);
    backFBO->bind();
    glContext->functions()->glViewport(0,0,500,500);
    QOpenGLFunctions *gl = glContext->functions();

    //int width = glContext->surface()->size().width();
    //int height = glContext->surface()->size().height();

    gl->glPixelStorei(GL_PACK_ALIGNMENT, 1);

    //  Load the identity.
    projection.setToIdentity();

    //projection.perspective(6.0f,1,1,6000);
    projection.perspective(glm::toDegrees((double)0.104719758f), 1.0f, 50.0f, 520.0f);

    gl->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
    //gl->glLoadIdentity();					// Reset The View
    modelview.setToIdentity();

    //back the camera up
    modelview.translate(0, 0, -480);

    //rotate camera so heading matched fix heading in the world
    //gl->glRotated(toDegrees(fixHeadingSection), 0, 0, 1);
    modelview.rotate(glm::toDegrees(vehicle.toolPos.heading), 0, 0, 1);

    //translate to that spot in the world
    modelview.translate(-vehicle.toolPos.easting, -vehicle.toolPos.northing, 0);

    //patch color
    QColor patchColor = QColor::fromRgbF(0.0f, 0.5f, 0.0f);

    //to draw or not the triangle patch
    bool isDraw;

    //calculate the frustum for the section control window
    calcFrustum(projection*modelview);

    //draw patches j= # of sections
    for (int j = 0; j < tool_numSuperSection; j++)
    {
        //every time the section turns off and on is a new patch
        int patchCount = tool.section[j].patchList.size();

        if (patchCount > 0)
        {
            //for every new chunk of patch
            foreach (QSharedPointer<QVector<QVector3D>> triList, tool.section[j].patchList)
            {
                isDraw = false;
                int count2 = triList->size();
                for (int i = 1; i < count2; i+=3)
                {
                    //determine if point is in frustum or not
                    //x is easting, y is northing
                    if (frustum[0] * (*triList)[i].x() + frustum[1] * (*triList)[i].y() + frustum[3] <= 0)
                        continue;//right
                    if (frustum[4] * (*triList)[i].x() + frustum[5] * (*triList)[i].y() + frustum[7] <= 0)
                        continue;//left
                   if (frustum[16] * (*triList)[i].x() + frustum[17] * (*triList)[i].y() + frustum[19] <= 0)
                        continue;//bottom
                    if (frustum[20] * (*triList)[i].x() + frustum[21] * (*triList)[i].y() + frustum[23] <= 0)
                        continue;//top

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

    //draw bright green on back buffer
    if (bnd.bndArr.count() > 0)
    {
        ////draw the bnd line
        int ptCount = bnd.bndArr[0].bndLine.count();
        if (ptCount > 1)
        {
            gl->glLineWidth(2);
            for (int h = 0; h < ptCount; h++)
                gldraw.append(QVector3D(bnd.bndArr[0].bndLine[h].easting, bnd.bndArr[0].bndLine[h].northing, 0));

            gldraw.draw( gl, projection*modelview, QColor::fromRgbF(0.0f, 0.99f, 0.0f),
                         GL_LINE_STRIP, 2.0);
        }
    }

    //draw the headland
    if (hd.isOn) hd.drawHeadLinesBack(gl, projection*modelview);

    //finish it up - we need to read the ram of video card
    gl->glFlush();

    //read the whole block of pixels up to max lookahead, one read only
    //we'll use Qt's QImage function to grab it.
    grnPix = backFBO->toImage().mirrored().convertToFormat(QImage::Format_RGBX8888);
    //qDebug() << grnPix.size();
    QImage temp = grnPix.copy(tool.rpXPosition, 250, tool.rpWidth, 245 /*(int)rpHeight*/);
    memcpy(grnPixels, temp.constBits(), temp.size().width() * temp.size().height() * 4);
    //grnPix = temp;

    //The remaining codesteerSlider the original method in the C# code is
    //broken out into a callback in formgps.c called
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

void  FormGPS::drawManUTurnBtn(QOpenGLFunctions *gl, QMatrix4x4 mvp)
{
    GLHelperTexture gldraw;
    VertexTexcoord vt;

    int two3 = qmlItem(qml_root, "openglcontrol")->property("width").toReal() / 6;
    //int two3 = 0;

    vt.texcoord = QVector2D(0,0); vt.vertex = QVector3D(-82 - two3, 45, 0);
    gldraw.append(vt);
    vt.texcoord = QVector2D(1,0); vt.vertex = QVector3D(82 - two3, 45, 0);
    gldraw.append(vt);
    vt.texcoord = QVector2D(0,1); vt.vertex = QVector3D(-82 - two3, 120, 0);
    gldraw.append(vt);
    vt.texcoord = QVector2D(1,1); vt.vertex = QVector3D(82 - two3, 120, 0);
    gldraw.append(vt);

    gldraw.draw(gl, mvp, Textures::TURNMANUAL, GL_TRIANGLE_STRIP, true, QColor::fromRgbF(0.90f, 0.90f, 0.293f));
}

void FormGPS::drawUTurnBtn(QOpenGLFunctions *gl, QMatrix4x4 mvp)
{
    QColor color;
    Textures whichtex;
    GLHelperTexture gldraw;
    VertexTexcoord vt;
    QLocale locale;

    if (!yt.isYouTurnTriggered)
    {
        whichtex = Textures::TURN;
        if (distancePivotToTurnLine > 0 && !yt.isOutOfBounds) color = QColor::fromRgbF(0.3f, 0.95f, 0.3f);
        else color = QColor::fromRgbF(0.97f, 0.635f, 0.4f);
    }
    else
    {
        whichtex = Textures::TURNCANCEL;
        color = QColor::fromRgbF(0.90f, 0.90f, 0.293f);
    }

    int two3 = qmlItem(qml_root, "openglcontrol")->property("width").toReal() / 5;
    if (!yt.isYouTurnRight)
    {
        vt.texcoord = QVector2D(0, 0); vt.vertex = QVector3D(-62 + two3, 50,0); //
        gldraw.append(vt);
        vt.texcoord = QVector2D(1, 0); vt.vertex = QVector3D(62 + two3, 50,0); //
        gldraw.append(vt);
        vt.texcoord = QVector2D(0, 1); vt.vertex = QVector3D(-62 + two3, 120,0); //
        gldraw.append(vt);
        vt.texcoord = QVector2D(1, 1); vt.vertex = QVector3D(62 + two3, 120,0); //
        gldraw.append(vt);
    }
    else
    {
        vt.texcoord = QVector2D(0, 0); vt.vertex = QVector3D(62 + two3, 50,0); //
        gldraw.append(vt);
        vt.texcoord = QVector2D(1, 0); vt.vertex = QVector3D(-62 + two3, 50,0); //
        gldraw.append(vt);
        vt.texcoord = QVector2D(0, 1); vt.vertex = QVector3D(62 + two3, 120,0); //
        gldraw.append(vt);
        vt.texcoord = QVector2D(1, 1); vt.vertex = QVector3D(-62 + two3, 120,0); //
        gldraw.append(vt);
    }

    gldraw.draw(gl, mvp, whichtex, GL_TRIANGLE_STRIP, true, color);

    // Done Building Triangle Strip
    if (SETTINGS_DISPLAY_ISMETRIC)
    {
        if (!yt.isYouTurnTriggered)
        {
            //drawText(gl, mvp, -30 + two3, 80, DistPivotM, 1.0, true, color);
        }
        else
        {
            drawText(gl, mvp, -30 + two3, 80, locale.toString(yt.onA), 1.0, true, color);
        }
    }
    else
    {

        if (!yt.isYouTurnTriggered)
        {
            //drawText(gl, mvp, -40 + two3, 85, DistPivotFt, 1.0, true, color);
        }
        else
        {
            drawText(gl, mvp, -40 + two3, 85, locale.toString(yt.onA), 1.0, true, color);
        }
    }

}

void FormGPS::makeFlagMark(QOpenGLFunctions *gl)
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

}

void FormGPS::drawFlags(QOpenGLFunctions *gl, QMatrix4x4 mvp)
{
    GLHelperOneColor gldraw;

    int flagCnt = flagPts.count();
    for (int f = 0; f < flagCnt; f++)
    {
        QColor color;
        if (flagPts[f].color == 0) color = QColor::fromRgb(255, 0, flagPts[f].ID);
        if (flagPts[f].color == 1) color = QColor::fromRgb(0, 255, flagPts[f].ID);
        if (flagPts[f].color == 2) color = QColor::fromRgb(255, 255, flagPts[f].ID);
        gldraw.append(QVector3D(flagPts[f].easting, flagPts[f].northing, 0));
        gldraw.draw(gl, mvp, color, GL_POINTS, 8.0f);

        drawText3D(camera, gl, mvp, flagPts[f].easting, flagPts[f].northing, "&" + (f+1));
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

void FormGPS::drawLightBar(QOpenGLFunctions *gl, QMatrix4x4 mvp, double Width, double Height, double offlineDistance)
{
    USE_SETTINGS;

    GLHelperColors gldraw;
    ColorVertex tv;

    double down = 20;
    gl->glLineWidth(1);

    //  Dot distance is representation of how far from AB Line
    int dotDistance = offlineDistance;
    int limit = SETTINGS_DISPLAY_LIGHTBARCMPP * 8;

    if (dotDistance < -limit) dotDistance = -limit;
    if (dotDistance > limit) dotDistance = limit;

    // dot background
    for (int i = -8; i < 0; i++) gldraw.append({ QVector3D(i*32, down, 0),
                                                QVector4D(0,0,0,1)} );
    for (int i = 1; i < 9; i++) gldraw.append({ QVector3D(i*32, down, 0),
                                               QVector4D(0,0,0,1)} );

    gldraw.draw(gl, mvp, GL_POINTS, 8.0f);

    gldraw.clear();
    //red left side
    for (int i = -8; i < 0; i++) gldraw.append({ QVector3D(i*32, down, 0),
                                                QVector4D(0.9750f, 0.0f, 0.0f, 1.0f) } );

    //green right side
    for (int i = 1; i < 9; i++) gldraw.append( { QVector3D(i*32, down, 0),
                                                QVector4D(0.0f, 0.9750f, 0.0f, 1.0f) } );

    gldraw.draw(gl, mvp, GL_POINTS, 4.0f);

    //Are you on the right side of line? So its green.
    if ((offlineDistance) < 0.0)
    {
        int numDots = dotDistance * -1 / SETTINGS_DISPLAY_LIGHTBARCMPP;

        gldraw.clear();
        for (int i = 1; i < numDots + 1; i++) gldraw.append({ QVector3D(i*32, down, 0),
                                                            QVector4D(0,0,0,1) } );
        gldraw.draw(gl, mvp, GL_POINTS, 32.0f);

        gldraw.clear();
        for (int i = 0; i < numDots; i++) gldraw.append({ QVector3D(i*32+32, down, 0),
                                                        QVector4D(0.0f, 0.980f, 0.0f,1)} );
        gldraw.draw(gl, mvp, GL_POINTS, 24.0f);
    }

    else
    {
        int numDots = dotDistance / SETTINGS_DISPLAY_LIGHTBARCMPP;

        gldraw.clear();
        for (int i = 1; i < numDots + 1; i++) gldraw.append({ QVector3D(i*-32, down, 0),
                                                            QVector4D(0,0,0,1) } );
        gldraw.draw(gl, mvp, GL_POINTS, 32.0f);

        gldraw.clear();
        for (int i = 0; i < numDots; i++) gldraw.append({ QVector3D(i*-32-32, down, 0),
                                                        QVector4D(0.980f, 0.30f, 0.0f,1)} );
        gldraw.draw(gl, mvp, GL_POINTS, 24.0f);
    }

    //yellow center dot
    ColorVertex p = { QVector3D(0, down, 0),
                      QVector4D(0,0,0,1) };

    gldraw.clear();
    gldraw.append(p);

    if (dotDistance >= -SETTINGS_DISPLAY_LIGHTBARCMPP && dotDistance <= SETTINGS_DISPLAY_LIGHTBARCMPP)
    {
        gldraw.draw(gl, mvp, GL_POINTS, 32.0f);

        gldraw[0].color = QVector4D(0.980f, 0.98f, 0.0f, 1.0f);
        gldraw.draw(gl, mvp, GL_POINTS, 24.0f);
    }

    else
    {
        gldraw.draw(gl, mvp, GL_POINTS, 8.0f);
    }

}

void FormGPS::drawLightBarText(QOpenGLFunctions *gl, QMatrix4x4 mvp, double Width, double Height)
{
    QLocale locale;
    if (ct.isContourBtnOn || ABLine.isBtnABLineOn || curve.isBtnCurveOn)
    {
        double dist = vehicle.distanceDisplay * 0.1;

        drawLightBar(gl, mvp, Width, Height, dist);

        double size = 1.5;
        QString hede;

        if (dist == 3200 || dist == 3202 )
        {
            //lblDistanceOffLine.Text = "Lost";
        }
        else
        {
            QColor color;
            if (dist < 0.0)
            {
                color = QColor::fromRgbF(0.50f, 0.952f, 0.3f);
                hede = QString("< ") + locale.toString(fabs(dist), 'f', 1);
            }
            else
            {
                color = QColor::fromRgbF(0.9752f, 0.50f, 0.3f);
                hede = locale.toString(dist, 'f', 1) + " >" ;
            }
            int center = -(int)(((double)(hede.length()) * 0.5) * 16 * size);
            drawText(gl, mvp, center, 38, hede, size, true, color);
        }
    }
}

void FormGPS::drawRollBar(QOpenGLFunctions *gl, QMatrix4x4 modelview, QMatrix4x4 projection)
{
    USE_SETTINGS;

    double set = vehicle.guidanceLineSteerAngle * 0.01 * (50 / SETTINGS_VEHICLE_MAXSTEERANGLE);
    double actual = actualSteerAngleDisp * 0.01 * (50 / SETTINGS_VEHICLE_MAXSTEERANGLE);
    double hiit = 0;
    GLHelperOneColor gldraw;

    modelview.translate(0, 100, 0); //will not override caller's modelview

    //If roll is used rotate graphic based on roll angle
    if ((SETTINGS_GPS_ISROLLFROMAUTOSTEER || SETTINGS_GPS_ISROLLFROMGPS || SETTINGS_GPS_ISROLLFROMOGI) && ahrs.rollX16 != 9999)
        modelview.rotate(((ahrs.rollX16 - SETTINGS_GPS_IMUROLLZEROX16) * 0.0625f), 0.0f, 0.0f, 1.0f);

    gl->glLineWidth(1);

    double wiid = 50;

    gldraw.append(QVector3D(-wiid, 25, 0));
    gldraw.append(QVector3D(-wiid, 0, 0));
    gldraw.append(QVector3D(wiid, 0, 0));
    gldraw.append(QVector3D(wiid, 25, 0));

    gldraw.draw(gl, projection*modelview, QColor::fromRgbF(0.54f, 0.54f, 0.54f),
                GL_LINE_STRIP, 1);

    modelview.translate(0, 10, 0);

    {
        if (actualSteerAngleDisp > 0)
        {
            gl->glLineWidth(1);

            gldraw.clear();
            gldraw.append(QVector3D(0, hiit, 0));
            gldraw.append(QVector3D(actual, hiit + 8, 0));
            gldraw.append(QVector3D(0, hiit + 16, 0));
            gldraw.append(QVector3D(0, hiit, 0));
            gldraw.draw(gl, projection*modelview, QColor::fromRgbF(0.0f, 0.75930f, 0.0f),
                        GL_LINE_STRIP, 1);

        }
        else
        {
            //actual
            gl->glLineWidth(1);

            gldraw.clear();
            gldraw.append(QVector3D(-0, hiit, 0));
            gldraw.append(QVector3D(actual, hiit + 8, 0));
            gldraw.append(QVector3D(-0, hiit + 16, 0));
            gldraw.append(QVector3D(-0, hiit, 0));
            gldraw.draw(gl, projection*modelview, QColor::fromRgbF(0.75930f, 0.0f, 0.0f),
                        GL_LINE_STRIP, 1);
        }
    }

    if (vehicle.guidanceLineSteerAngle > 0)
    {
        gl->glLineWidth(1);

        gldraw.clear();
        gldraw.append(QVector3D(0, hiit, 0));
        gldraw.append(QVector3D(set, hiit + 8, 0));
        gldraw.append(QVector3D(0, hiit + 16, 0));
        gldraw.append(QVector3D(0, hiit, 0));
        gldraw.draw(gl, projection*modelview, QColor::fromRgbF(0.75930f, 0.75930f, 0.0f),
                    GL_LINE_STRIP, 1);

    }
    else
    {
        gl->glLineWidth(1);

        gldraw.clear();
        gldraw.append(QVector3D(-0, hiit, 0));
        gldraw.append(QVector3D(set, hiit + 8, 0));
        gldraw.append(QVector3D(-0, hiit + 16, 0));
        gldraw.append(QVector3D(-0, hiit, 0));
        gldraw.draw(gl, projection*modelview, QColor::fromRgbF(0.75930f, 0.75930f, 0.0f),
                    GL_LINE_STRIP, 1);

    }

    //return back
}


void FormGPS::drawSky(QOpenGLFunctions *gl, QMatrix4x4 mvp, int width, int height)
{
    USE_SETTINGS;

    VertexTexcoord vertices[4];
    double camPitch = SETTINGS_DISPLAY_CAMPITCH;

    ////draw the background when in 3D
    if (camPitch < -52)
    {
        if ( (lastWidth != width)  || (lastHeight != height)) {
            lastWidth = width;
            lastHeight = height;

            //-10 to -32 (top) is camera pitch range. Set skybox to line up with horizon
            double hite = (camPitch + 63) * -0.026;
            //hite = 0.001;

            //the background
            double winLeftPos = -(double)width / 2;
            double winRightPos = -winLeftPos;


            //map texture coordinates to model coordinates
            vertices[0].vertex = QVector3D(winRightPos, 0.0, 0);
            vertices[0].texcoord = QVector2D(0,0); //top right

            vertices[1].vertex = QVector3D(winLeftPos, 0.0, 0);
            vertices[1].texcoord = QVector2D(1,0); //top left

            vertices[2].vertex = QVector3D(winRightPos, hite*(double)height,0);
            vertices[2].texcoord = QVector2D(0,1); //bottom right

            vertices[3].vertex = QVector3D(winLeftPos, hite*(double)height,0);
            vertices[3].texcoord = QVector2D(1,1); //bottom left

            //rebuild sky buffer
            if (skyBuffer.isCreated())
                skyBuffer.destroy();
            skyBuffer.create();
            skyBuffer.bind();
            skyBuffer.allocate(vertices,4*sizeof(VertexTexcoord));
            //skyBuffer.allocate(vertices,4*sizeof(QVector3D));
            skyBuffer.release();
        }

        texture[Textures::SKY]->bind(0);
        //TODO nighttime sky

        glDrawArraysTexture(gl, mvp,
                            GL_TRIANGLE_STRIP, skyBuffer,
                            GL_FLOAT,
                            4, true, QColor::fromRgbF(0.5,0.5,0.5));
        texture[Textures::SKY]->release();

    }
}

void FormGPS::drawCompassText(QOpenGLFunctions *gl, QMatrix4x4 mvp, double Width)
{
    USE_SETTINGS;
    QLocale locale;
    QString hede = locale.toString(camera.camHeading, 'f', 1);
    int center = Width / 2 - 45 - (int)(((double)(hede.length()) * 0.5) * 16);

    if (SETTINGS_DISPLAY_COMPASS)
        drawText(gl, mvp, center, 65, hede, 0.8, true, QColor::fromRgbF(0.9752f, 0.952f, 0.83f));
    else drawText(gl, mvp, center, 65, hede, 1.2, true, QColor::fromRgbF(0.9752f, 0.952f, 0.83f));
}

void FormGPS::drawCompass(QOpenGLFunctions *gl, QMatrix4x4 modelview, QMatrix4x4 projection, double Width)
{
    //Heading text
    int center = Width / 2 - 55;
    drawText(gl, projection*modelview, center-8, 40, "^", 0.8);

    GLHelperTexture gldraw;


    modelview.translate(center, 78, 0);

    modelview.rotate(-camera.camHeading, 0, 0, 1);
    gldraw.append( { QVector3D(-52, -52, 0), QVector2D(0, 0) }); //bottom left
    gldraw.append( { QVector3D(52, -52.0, 0), QVector2D(1, 0) }); //bottom right
    gldraw.append( { QVector3D(-52, 52, 0), QVector2D(0, 1) }); // top left
    gldraw.append( { QVector3D(52, 52, 0), QVector2D(1, 1) }); // top right

    gldraw.draw(gl, projection*modelview, Textures::COMPASS, GL_TRIANGLE_STRIP, true, QColor::fromRgbF(0.952f, 0.870f, 0.73f, 0.8f));
}

void FormGPS::drawSpeedo(QOpenGLFunctions *gl, QMatrix4x4 modelview, QMatrix4x4 projection, double Width, double Height)
{
    GLHelperTexture gldraw1;

    int bottomSide = Height - 55;

    modelview.translate(Width / 2 - 60, bottomSide, 0);

    gldraw1.append({ QVector3D(-58, -58, 0), QVector2D(0, 0) }); //bottom left
    gldraw1.append({ QVector3D(58, -58.0, 0), QVector2D(1, 0) }); //bottom right
    gldraw1.append({ QVector3D(-58, 58, 0), QVector2D(0, 1) }); //top left
    gldraw1.append({ QVector3D(58, 58, 0), QVector2D(1, 1) }); //top right

    gldraw1.draw(gl, projection*modelview, Textures::SPEEDO, GL_TRIANGLE_STRIP, true, QColor::fromRgbF(0.952f, 0.870f, 0.823f, 0.8));

    double angle = 0;
    double aveSpd = 0;
    if (SETTINGS_DISPLAY_ISMETRIC)
    {
        aveSpd = fabs(vehicle.avgSpeed);
        if (aveSpd > 20) aveSpd = 20;
        angle = (aveSpd - 10) * 15;
    }
    else
    {
        double aveSpd = fabs(vehicle.avgSpeed) * 0.621371;
        if (aveSpd > 20) aveSpd = 20;
        angle = (aveSpd - 10) * 15;
    }

    QColor color;
    if (vehicle.avgSpeed > -0.1) color = QColor::fromRgbF(0.0f, 0.950f, 0.0f);
    else color = QColor::fromRgbF(0.952f, 0.0f, 0.0f);

    gldraw1.clear();
    modelview.rotate(angle, 0, 0, 1);
    gldraw1.append({ QVector3D(-48, -48, 0),  QVector2D(0, 0) });
    gldraw1.append({ QVector3D(48, -48.0, 0), QVector2D(1, 0) });
    gldraw1.append({ QVector3D(-48, 48, 0),   QVector2D(0, 1) });
    gldraw1.append({ QVector3D(48, 48, 0),    QVector2D(1, 1) });

    gldraw1.draw(gl, projection*modelview, Textures::SPEEDONEDLE, GL_TRIANGLE_STRIP,
                 true, color);
}

void FormGPS::drawLiftIndicator(QOpenGLFunctions *gl, QMatrix4x4 modelview, QMatrix4x4 projection, int Width, int Height)
{
    modelview.translate(Width / 2 - 35, Height/2, 0);
    QColor color;

    GLHelperTexture gldraw;

    if (mc.machineData[mc.mdHydLift] == 2)
    {
        color = QColor::fromRgbF(0.0f, 0.950f, 0.0f);
    }
    else
    {
        modelview.rotate(180, 0, 0, 1);
        color = QColor::fromRgbF(0.952f, 0.40f, 0.0f);
    }

    gldraw.append({ QVector3D(-48, -64, 0),  QVector2D(0, 0) });  //
    gldraw.append({ QVector3D(-48, 64, 0),   QVector2D(0, 1) }); //
    gldraw.append({ QVector3D(48, -64.0, 0), QVector2D(1, 0) }); //
    gldraw.append({ QVector3D(48, 64, 0),    QVector2D(1, 1) }); //

    gldraw.draw(gl, projection * modelview, Textures::HYDLIFT,
                GL_TRIANGLE_STRIP, true, color);
}

void FormGPS::calcFrustum(const QMatrix4x4 &mvp)
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
    if (bnd.bndArr.count() > 0)
    {
        int bndCnt = bnd.bndArr[0].bndLine.count();
        for (int i = 0; i < bndCnt; i++)
        {
            double x = bnd.bndArr[0].bndLine[i].easting;
            double y = bnd.bndArr[0].bndLine[i].northing;

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
        for (int j = 0; j < SETTINGS_TOOL_NUMSECTIONS; j++)
        {
            //every time the section turns off and on is a new patch
            int patchCount = tool.section[j].patchList.count();

            if (patchCount > 0)
            {
                //for every new chunk of patch
                QSharedPointer<TriangleList> triList;

                foreach (triList, tool.section[j].patchList)
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

}
