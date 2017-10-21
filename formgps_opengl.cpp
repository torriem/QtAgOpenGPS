//#include <QtOpenGL>
#include <assert.h>
#include "ui_formgps.h"
#include "formgps.h"
#include "csection.h"
#include "cvehicle.h"
#include "cworldgrid.h"
#include "ccontour.h"
#include "cabline.h"
#include "cperimeter.h"
#include "cboundary.h"
#include "openglcontrol.h"
#include "cnmea.h"
#include "glm.h"

#include <QGLWidget>
#include <QQuickView>
//#include <QOpenGLFunctions_1_1>
#include <QOpenGLFunctions_2_1>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include <assert.h>

#include <iostream>

//Wrapper to draw individual primitives. Not very efficient but
//if you need to just draw a few lines or points, this can be
//used.  Data should a long list of 2 floats?
void FormGPS::glDrawArraysColor(QOpenGLFunctions *gl,
                                QMatrix4x4 mvp,
                                GLenum operation,
                                QColor &color,
                                QOpenGLBuffer &vertexBuffer,
                                GLenum GL_type,
                                int count,
                                float pointSize)
{
    //bind shader
    assert(simpleColorShader->bind());
    //set color
    simpleColorShader->setUniformValue("color", color);
    //set mvp matrix
    simpleColorShader->setUniformValue("mvpMatrix", mvp);

    simpleColorShader->setUniformValue("pointSize", pointSize);


    vertexBuffer.bind();

    //enable the vertex attribute array in shader
    simpleColorShader->enableAttributeArray("vertex");
    //use attribute array from buffer, using non-normalized vertices
    gl->glVertexAttribPointer(simpleColorShader->attributeLocation("vertex"),
                              2, //2D vertices
                              GL_type, //type of data GL_FLAOT or GL_DOUBLE
                              GL_FALSE, //not normalized vertices!
                              0, //no spaceing between vertices in data
                              0 //start at offset 0 in buffer
                             );

    //draw primitive
    gl->glDrawArrays(operation,0,count);
    //release buffer
    vertexBuffer.release();
    //release shader
    simpleColorShader->release();
}


void FormGPS::openGLControl_Draw()
{
    QOpenGLContext *glContext = QOpenGLContext::currentContext();
    QOpenGLFunctions_2_1 *gl = glContext->versionFunctions<QOpenGLFunctions_2_1>();
    QOpenGLFunctions *gles = glContext->functions();
    int width = glContext->surface()->size().width();
    int height = glContext->surface()->size().height();
    QMatrix4x4 projection;
    QMatrix4x4 modelview;

    gl->glEnable(GL_POINT_SPRITE);
    gl->glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    //std::cout << "draw routine here." << std::endl;

    //Do stuff that was in the initialized method, since Qt uses OpenGL and may
    //have messed with the matrix stuff and other defaults

    //qml has messed with the state variables; reset them to a sane state.
    qmlview->resetOpenGLState();
    //  Set the clear color.

    //gl->glClearColor(0.22f, 0.2858f, 0.16f, 1.0f);

    // Set The Blending Function For Translucency
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glDisable(GL_CULL_FACE);
    gl->glCullFace(GL_BACK);

    //set the camera to right distance
    setZoom();

    //now move the "camera" to the calculated zoom settings
    //I had to move these functions here because if setZoom is called
    //from elsewhere in the GUI (say a button press), there's no GL
    //context to work with.
    gl->glMatrixMode(GL_PROJECTION);
    gl->glLoadIdentity();

    projection.setToIdentity();

    //  Create a perspective transformation.
    //gl.Perspective(fovy, openGLControl.Width / (double)openGLControl.Height, 1, camDistanceFactor * camera.camSetDistance);
    double aspect = width / height;
    double fH = tan( fovy / 360 * M_PI);
    double fW = fH * aspect;

    gl->glFrustum(-fW, fW, -fH, fH, 1, camDistanceFactor * camera.camSetDistance );
    projection.frustum(-fW,fW,-fH,fH, 1, camDistanceFactor * camera.camSetDistance);

    //  Set the modelview matrix.
    gl->glMatrixMode(GL_MODELVIEW);

    //default MODELVIEW matrix is identity


    if (isGPSPositionInitialized)
    {

        //  Clear the color and depth buffer.
        gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gl->glLoadIdentity();
        modelview.setToIdentity();

        //camera does translations and rotations
        camera.setWorldCam(gl, pivotAxlePos.easting, pivotAxlePos.northing, fixHeadingCam);
        //do our own matrices
        camera.setWorldCam(modelview, pivotAxlePos.easting, pivotAxlePos.northing, fixHeadingCam);

        //calculate the frustum planes for culling
        calcFrustum(projection, modelview);

        //draw the field ground images
        worldGrid->drawFieldSurface(gl);

        ////Draw the world grid based on camera position
        gl->glDisable(GL_DEPTH_TEST);
        gl->glDisable(GL_TEXTURE_2D);

        ////if grid is on draw it
        if (isGridOn) worldGrid->drawWorldGrid(gles,projection*modelview,gridZoom);

        //turn on blend for paths
        gl->glEnable(GL_BLEND);

        //section patch color
        QColor sectionColor = QColor(settings.value("display/sectionColor", "#32DCC8").toString());
        gl->glColor4ub(sectionColor.red(), sectionColor.green(), sectionColor.blue(),160);
        if (isDrawPolygons) gl->glPolygonMode(GL_FRONT, GL_LINE);

        //draw patches of sections
        for (int j = 0; j < vehicle->numSuperSection; j++)
        {
            //every time the section turns off and on is a new patch
            int patchCount = section[j].patchList.size();

            //check if in frustum or not
            bool isDraw;

            if (patchCount > 0)
            {
                //initialize the steps for mipmap of triangles (skipping detail while zooming out)
                int mipmap = 0;
                if (camera.camSetDistance < -800) mipmap = 2;
                if (camera.camSetDistance < -1500) mipmap = 4;
                if (camera.camSetDistance < -2400) mipmap = 8;
                if (camera.camSetDistance < -4800) mipmap = 16;

                //for every new chunk of patch

                //foreach is a Qt macro that iterates over Qt containers
                foreach (QSharedPointer<QVector<Vec2>> triList, section[j].patchList)
                {
                    isDraw = false;
                    int count2 = triList->size();
                    for (int i = 0; i < count2; i += 3)
                    {
                        //determine if point is in frustum or not, if < 0, its outside so abort, z always is 0
                        if (frustum[0] * (*triList)[i].easting + frustum[1] * (*triList)[i].northing + frustum[3] <= 0)
                            continue;//right
                        if (frustum[4] * (*triList)[i].easting + frustum[5] * (*triList)[i].northing + frustum[7] <= 0)
                            continue;//left
                        if (frustum[16] * (*triList)[i].easting + frustum[17] * (*triList)[i].northing + frustum[19] <= 0)
                            continue;//bottom
                        if (frustum[20] * (*triList)[i].easting + frustum[21] * (*triList)[i].northing + frustum[23] <= 0)
                            continue;//top
                        if (frustum[8] * (*triList)[i].easting + frustum[9] * (*triList)[i].northing + frustum[11] <= 0)
                            continue;//far
                        if (frustum[12] * (*triList)[i].easting + frustum[13] * (*triList)[i].northing + frustum[15] <= 0)
                            continue;//near

                        //point is in frustum so draw the entire patch. The downside of triangle strips.
                        isDraw = true;
                        break;
                    }

                    if (isDraw)
                    {
                        //draw the triangle in each triangle strip
                        gl->glBegin(GL_TRIANGLE_STRIP);
                        count2 = triList->size();

                        //if large enough patch and camera zoomed out, fake mipmap the patches, skip triangles
                        if (count2 >= (mipmap + 2))
                        {
                            int step = mipmap;
                            for (int i = 0; i < count2; i += step)
                            {
                                gl->glVertex3d((*triList)[i].easting, (*triList)[i].northing, 0); i++;
                                gl->glVertex3d((*triList)[i].easting, (*triList)[i].northing, 0); i++;

                                //too small to mipmap it
                                if (count2 - i <= (mipmap + 2)) step = 0;
                            }
                        } else {
                            for (int i = 0; i < count2; i++)
                                gl->glVertex3d((*triList)[i].easting, (*triList)[i].northing, 0);
                        }
                        gl->glEnd();
                    }
                }
            }
        }


        gl->glPolygonMode(GL_FRONT, GL_FILL);
        gl->glColor3f(1, 1, 1);

        //draw contour line if button on
        if (ct->isContourBtnOn) ct->drawContourLine(glContext);

        // draw the current and reference AB Lines
        else { if (ABLine->isABLineSet || ABLine->isABLineBeingSet) ABLine->drawABLines(glContext); }

        //draw the flags if there are some
        int flagCnt = flagPts.size();
        if (flagCnt > 0)
        {
            for (int f = 0; f < flagCnt; f++)
            {
                gl->glPointSize(8.0f);
                gl->glBegin(GL_POINTS);
                if (flagPts[f].color == 0) gl->glColor3ub(255, 0, flagPts[f].ID);
                if (flagPts[f].color == 1) gl->glColor3ub(0, 255, flagPts[f].ID);
                if (flagPts[f].color == 2) gl->glColor3ub(255, 255, flagPts[f].ID);
                gl->glVertex3d(flagPts[f].easting, flagPts[f].northing, 0);
                gl->glEnd();
            }

            if (flagNumberPicked != 0)
            {
                ////draw the box around flag
                gl->glLineWidth(4);
                gl->glColor3f(0.980f, 0.0f, 0.980f);
                gl->glBegin(GL_LINE_STRIP);

                double offSet = (zoomValue * zoomValue * 0.01);
                gl->glVertex3d(flagPts[flagNumberPicked - 1].easting, flagPts[flagNumberPicked - 1].northing + offSet, 0);
                gl->glVertex3d(flagPts[flagNumberPicked - 1].easting - offSet, flagPts[flagNumberPicked - 1].northing, 0);
                gl->glVertex3d(flagPts[flagNumberPicked - 1].easting, flagPts[flagNumberPicked - 1].northing - offSet, 0);
                gl->glVertex3d(flagPts[flagNumberPicked - 1].easting + offSet, flagPts[flagNumberPicked - 1].northing, 0);
                gl->glVertex3d(flagPts[flagNumberPicked - 1].easting, flagPts[flagNumberPicked - 1].northing + offSet, 0);

                gl->glEnd();

                //draw the flag with a black dot inside
                gl->glPointSize(4.0f);
                gl->glColor3f(0, 0, 0);
                gl->glBegin(GL_POINTS);
                gl->glVertex3d(flagPts[flagNumberPicked - 1].easting, flagPts[flagNumberPicked - 1].northing, 0);
                gl->glEnd();
            }
        }

        //draw the perimter line, returns if no line to draw
        periArea.drawPerimeterLine(glContext);

        //draw the boundary line
        boundary->drawBoundaryLine(gl);

        //screen text for debug
        //glDrawText(120, 10, 1, 1, 1, "Courier", 18, " camstep: " + testInt.ToString());
        //glDrawText(120, 40, 1, 1, 1, "Courier", 18, "head: " + Convert.ToString((double)(testDouble)));
        //glDrawText(120, 70, 1, 1, 1, "Courier", 18, " Xe: " + Convert.ToString(Xe));

        //glDrawText(40, 75, 1, 1, 1, "Courier", 16, " SteerCT " + Convert.ToString(ct.steeringAngleCT));
        //glDrawText(40, 90, 1, 1, 1, "Courier", 12, " RadiusCT " + Convert.ToString(ct.radiusCT));
        //glDrawText(40, 105, 1, 0.5f, 1, "Courier", 12, " TrigSetDist(m) " + Convert.ToString(Math.Round(sectionTriggerStepDistance, 2)));
        //glDrawText(40, 120, 1, 0.5, 1, "Courier", 12, " frame msec " + Convert.ToString((int)(frameTime)));

        //draw the vehicle/implement
        vehicle->drawVehicle(glContext,modelview, projection);

        //Back to normal
        gl->glColor3f(0.98f, 0.98f, 0.98f);
        gl->glDisable(GL_BLEND);
        gl->glEnable(GL_DEPTH_TEST);
        //// 2D Ortho --------------------------
        gl->glMatrixMode(GL_PROJECTION);
        gl->glPushMatrix();
        gl->glLoadIdentity();

        //negative and positive on width, 0 at top to bottom ortho view
        //should this be the width of the opengl control?
        gl->glOrtho(-(double)width / 2, width / 2, (double)height, 0, -1, 1);

        //  Create the appropriate modelview matrix.
        gl->glMatrixMode(GL_MODELVIEW);
        gl->glPushMatrix();
        gl->glLoadIdentity();

        if (ui->actionSky_On->isChecked())
        {
            ////draw the background when in 3D
            if (camera.camPitch < -60)
            {
                //-10 to -32 (top) is camera pitch range. Set skybox to line up with horizon
                double hite = (camera.camPitch + 60) / -20 * 0.34;
                //hite = 0.001;

                //the background
                double winLeftPos = -(double)width / 2;
                double winRightPos = -winLeftPos;
                gl->glEnable(GL_TEXTURE_2D);
                texture1[0]->bind();
                //gl->glBindTexture(GL_TEXTURE_2D, texture[0]);		// Select Our Texture

                gl->glBegin(GL_TRIANGLE_STRIP);				// Build Quad From A Triangle Strip
                gl->glTexCoord2i(0, 1); gl->glVertex2d(winRightPos, 0.0); // Top Right
                gl->glTexCoord2i(1, 1); gl->glVertex2d(winLeftPos, 0.0); // Top Left
                gl->glTexCoord2i(0, 0); gl->glVertex2d(winRightPos, hite * height); // Bottom Right
                gl->glTexCoord2i(1, 0); gl->glVertex2d(winLeftPos, hite * height); // Bottom Left
                gl->glEnd();						// Done Building Triangle Strip

                //disable, straight color
                //gl->glBindTexture(GL_TEXTURE_2D, 0); //unbind the texture
                texture1[0]->release();
                gl->glDisable(GL_TEXTURE_2D);
            }
        }

        //LightBar if AB Line is set and turned on or contour
        if (isLightbarOn)
        {
            if (ct->isContourBtnOn)
            {
                QString dist;
                //turn on distance widget
                //TODO: tie into GUI
                //txtDistanceOffABLine.Visible = true;
                //lblDelta.Visible = true;
                if (ct->distanceFromCurrentLine == 32000) ct->distanceFromCurrentLine = 0;

                //drawLightBar(gl,openGLControl->viewportSize.width(), openGLControl->viewportSize.height(), ct->distanceFromCurrentLine * 0.1);
                drawLightBar(gl,width, height, ct->distanceFromCurrentLine * 0.1);
                if ((ct->distanceFromCurrentLine) < 0.0)
                {
                    txtDistanceOffABLine->setProperty("color","green");

                    if (isMetric) dist = QString((int)fabs(ct->distanceFromCurrentLine * 0.1)) + " " + QChar(0x2192);
                    else dist = QString((int)fabs(ct->distanceFromCurrentLine / 2.54 * 0.1)) + " " + QChar(0x2192);
                    txtDistanceOffABLine->setProperty("text", dist);
                }

                else
                {
                    txtDistanceOffABLine->setProperty("color", "red");
                    if (isMetric) dist = QString("") + QChar(0x2190) + " " + ((int)fabs(ct->distanceFromCurrentLine * 0.1));
                    else dist = QString("") + QChar(0x2190)+ " " + ((int)fabs(ct->distanceFromCurrentLine / 2.54 * 0.1));
                    txtDistanceOffABLine->setProperty("text", dist);
                }

                //if (guidanceLineHeadingDelta < 0) lblDelta.ForeColor = Color.Red;
                //else lblDelta.ForeColor = Color.Green;

                if (guidanceLineDistanceOff == 32020 || guidanceLineDistanceOff == 32000)
                    btnAutoSteer->setProperty("buttonText", QChar(0x2715));
                else
                    btnAutoSteer->setProperty("buttonText", QChar(0x2713));
            }

            else
            {
                if (ABLine->isABLineSet || ABLine->isABLineBeingSet)
                {
                    QString dist;

                    txtDistanceOffABLine->setProperty("visible", "true");
                    //lblDelta.Visible = true;
                    drawLightBar(gl,
                                 width,
                                 height,
                                 ABLine->distanceFromCurrentLine * 0.1);
                    if ((ABLine->distanceFromCurrentLine) < 0.0)
                    {
                        // --->
                        txtDistanceOffABLine->setProperty("color","green");
                        if (isMetric) dist = QString((int)fabs(ABLine->distanceFromCurrentLine * 0.1)) + " \u21D2";
                        else dist = QString((int)fabs(ABLine->distanceFromCurrentLine / 2.54 * 0.1)) + " \u21D2";
                        txtDistanceOffABLine->setProperty("text", dist);
                    }

                    else
                    {
                        // <----
                        txtDistanceOffABLine->setProperty("color", "red");
                        if (isMetric) dist = QChar(0x21D0) + QString((int)fabs(ABLine->distanceFromCurrentLine * 0.1));
                        else dist = QChar(0x21D0) + QString((int)fabs(ABLine->distanceFromCurrentLine / 2.54 * 0.1));
                        txtDistanceOffABLine->setProperty("text", dist);
                    }

                    //if (guidanceLineHeadingDelta < 0) lblDelta.ForeColor = Color.Red;
                    //else lblDelta.ForeColor = Color.Green;
                    if (guidanceLineDistanceOff == 32020 || guidanceLineDistanceOff == 32000)
                        btnAutoSteer->setProperty("buttonText", QChar(0x2715));
                    else
                        btnAutoSteer->setProperty("buttonText", QChar(0x2713));
                }
            }

            //AB line is not set so turn off numbers
            if (!ABLine->isABLineSet && !ABLine->isABLineBeingSet && !ct->isContourBtnOn)
            {
                //**txtDistanceOffABLine->setProperty("visible", "false");
                //lblDelta.Visible = false;
                //**btnAutoSteer->setProperty("buttonText","-");
            }
        }

        else
        {
            txtDistanceOffABLine->setProperty("visible", "false");
            //lblDelta.Visible = false;
            btnAutoSteer->setProperty("buttonText","-");
        }

        gl->glFlush();//finish openGL commands
        gl->glPopMatrix();//  Pop the modelview.

        //  back to the projection and pop it, then back to the model view.
        gl->glMatrixMode(GL_PROJECTION);
        gl->glPopMatrix();
        gl->glMatrixMode(GL_MODELVIEW);

        //reset point size
        gl->glPointSize(1.0f);
        gl->glFlush();

        if (leftMouseDownOnOpenGL)
        {
            leftMouseDownOnOpenGL = false;
            uchar data1[192];

            //scan the center of click and a set of square points around
            gl->glReadPixels(mouseX - 4, mouseY - 4, 8, 8, GL_RGB, GL_UNSIGNED_BYTE, data1);

            //made it here so no flag found
            flagNumberPicked = 0;

            for (int ctr = 0; ctr < 192; ctr += 3)
            {
                if ((data1[ctr] == 255) || (data1[ctr + 1] == 255))
                {
                    flagNumberPicked = data1[ctr + 2];
                    break;
                }
            }
        }
    }
    else
    {
        gl->glClear(GL_COLOR_BUFFER_BIT);
    }
    qmlview->resetOpenGLState();
}

/// Handles the OpenGLInitialized event of the openGLControl control.
void FormGPS::openGLControl_Initialized()
{
    QOpenGLContext *glContext = QOpenGLContext::currentContext();
    QOpenGLFunctions_2_1 *gl = glContext->versionFunctions<QOpenGLFunctions_2_1>();

    qmlview->resetOpenGLState();

    //Load all the textures
    qDebug() << "initializing Open GL.";
    loadGLTextures(gl);
    qDebug() << "textures loaded.";

    //load shaders, memory managed by parent thread, which is in this case,
    //the QML rendering thread, not the Qt main loop.
    if (!simpleColorShader) {
        simpleColorShader = new QOpenGLShaderProgram(QThread::currentThread()); //memory managed by Qt
        assert(simpleColorShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/color_vshader.vsh"));
        assert(simpleColorShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/color_fshader.fsh"));
        assert(simpleColorShader->link());
    }
    if (!gridShader) {
        gridShader = new QOpenGLShaderProgram(QThread::currentThread()); //memory managed by Qt
        assert(gridShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/worldgrid.vsh"));
        assert(gridShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/color_fshader.fsh"));
        assert(gridShader->link());
    }
    //now start the timer assuming no errors, otherwise the program will not stop on errors.
    //TODO:
    //tmrWatchdog.Enabled = true;
    qmlview->resetOpenGLState();
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
    QOpenGLFunctions_2_1 *gl = glContext->versionFunctions<QOpenGLFunctions_2_1>();
    QMatrix4x4 projection;
    QMatrix4x4 modelview;

    int width = glContext->surface()->size().width();
    int height = glContext->surface()->size().height();

    gl->glPixelStorei(GL_PACK_ALIGNMENT, 1);


    /* set projection */
    gl->glMatrixMode(GL_PROJECTION);

    //  Load the identity.
    gl->glLoadIdentity();
    projection.setToIdentity();

    // change these at your own peril!!!! Very critical
    //  Create a perspective transformation.
    //gl->glPerspective(6.0f, 1, 1, 6000);
    double fH = tan( 6.0 / 360 * M_PI);
    double fW = fH;
    gl->glFrustum(-fW, fW, -fH, fH, 1, 6000 );
    projection.frustum(-fW, fW, -fH, fH, 1, 6000 );

    /* end set projection */

    //  Set the modelview matrix.
    gl->glMatrixMode(GL_MODELVIEW);

    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
    gl->glLoadIdentity();					// Reset The View
    modelview.setToIdentity();

    //back the camera up
    gl->glTranslated(0, 0, -390);
    modelview.translate(0, 0, -390);

    //rotate camera so heading matched fix heading in the world
    gl->glRotated(toDegrees(fixHeadingSection), 0, 0, 1);
    modelview.rotate(toDegrees(fixHeadingSection), 0, 0, 1);

    //translate to that spot in the world
    gl->glTranslated(-toolPos.easting, -toolPos.northing, -fixZ);
    modelview.translate(-toolPos.easting, -toolPos.northing, -fixZ);

    //patch color
    gl->glColor3f(0.0f, 0.5f, 0.0f);

    //calculate the frustum for the section control window
    calcFrustum(projection, modelview);

    //to draw or not the triangle patch
    bool isDraw;

    //draw patches j= # of sections
    for (int j = 0; j < vehicle->numSuperSection; j++)
    {
        //every time the section turns off and on is a new patch
        int patchCount = section[j].patchList.size();

        if (patchCount > 0)
        {
            //for every new chunk of patch
            foreach (QSharedPointer<QVector<Vec2>> triList, section[j].patchList)
            {
                isDraw = false;
                int count2 = triList->size();
                for (int i = 0; i < count2; i+=3)
                {
                    //determine if point is in frustum or not
                    if (frustum[0] * (*triList)[i].easting + frustum[1] * (*triList)[i].northing + frustum[3] <= 0)
                        continue;//right
                    if (frustum[4] * (*triList)[i].easting + frustum[5] * (*triList)[i].northing + frustum[7] <= 0)
                        continue;//left
                   if (frustum[16] * (*triList)[i].easting + frustum[17] * (*triList)[i].northing + frustum[19] <= 0)
                        continue;//bottom
                    if (frustum[20] * (*triList)[i].easting + frustum[21] * (*triList)[i].northing + frustum[23] <= 0)
                        continue;//top

                    //point is in frustum so draw the entire patch
                    isDraw = true;
                    break;
                }

                if (isDraw)
                {
                    //draw the triangles in each triangle strip
                    gl->glBegin(GL_TRIANGLE_STRIP);
                    for (int i = 0; i < count2; i++)
                        gl->glVertex3d((*triList)[i].easting, (*triList)[i].northing, 0);
                    gl->glEnd();
                }
            }
        }
    }

    //draw boundary line
    boundary->drawBoundaryLineOnBackBuffer(gl);

    //determine farthest ahead lookahead - is the height of the readpixel line
    double rpHeight = 0;

    //section state stuff is moved to processSectionLookAhead().

    //find the farthest lookahead
    for (int j = 0; j < vehicle->numOfSections; j++)
    {
        if (section[j].sectionLookAhead > rpHeight) rpHeight = section[j].sectionLookAhead;
    }

    //clamp the height after looking way ahead, this is for switching off super section only
    rpHeight = fabs(rpHeight) * 2.0;
    if (rpHeight > 195) rpHeight = 195;
    if (rpHeight < 8) rpHeight = 8;

    //read the whole block of pixels up to max lookahead, one read only
    gl->glReadPixels(vehicle->rpXPosition, 202, vehicle->rpWidth, (int)rpHeight,
                        GL_GREEN, GL_UNSIGNED_BYTE, grnPixels);

    grnPix = QImage(grnPixels,vehicle->rpWidth,rpHeight,vehicle->rpWidth,QImage::Format_Grayscale8);

    //The remaining code from the original method in the C# code is
    //broken out into a callback in formgps.c called
    //processSectionLookahead().

    gl->glFlush();
}

/*
//Resize is called upn window creation
private void openGLControlBack_Resized(object sender, EventArgs e)
{
    //  Get the OpenGL object.
    OpenGL gls = openGLControlBack.OpenGL;

    gl->glMatrixMode(OpenGL.GL_PROJECTION);

    //  Load the identity.
    gl->glLoadIdentity();

    // change these at your own peril!!!! Very critical
    //  Create a perspective transformation.
    gl->glPerspective(6.0f, 1, 1, 6000);

    //  Set the modelview matrix.
    gl->glMatrixMode(OpenGL.GL_MODELVIEW);
}

*/
//Draw section OpenGL window, not visible
void FormGPS::openGLControlBack_Initialized()
{
}

void FormGPS::drawLightBar(QOpenGLFunctions_2_1 *gl, double Width, double Height, double offlineDistance)
{
    //QOpenGLFunctions_2_1 *gl = glContext->versionFunctions<QOpenGLFunctions_2_1>();
    double down = 20;

    gl->glLineWidth(1);

    //  Dot distance is representation of how far from AB Line
    int dotDistance = (int)(offlineDistance);

    if (dotDistance < -320) dotDistance = -320;
    if (dotDistance > 320) dotDistance = 320;

    if (dotDistance < -10) dotDistance -= 30;
    if (dotDistance > 10) dotDistance += 30;

    // dot background
    gl->glPointSize(8.0f);
    gl->glColor3f(0.00f, 0.0f, 0.0f);
    gl->glBegin(GL_POINTS);
    for (int i = -10; i < 0; i++) gl->glVertex2d((i * 40), down);
    for (int i = 1; i < 11; i++) gl->glVertex2d((i * 40), down);
    gl->glEnd();

    gl->glPointSize(4.0f);

    //red left side
    gl->glColor3f(0.9750f, 0.0f, 0.0f);
    gl->glBegin(GL_POINTS);
    for (int i = -10; i < 0; i++) gl->glVertex2d((i * 40), down);

    //green right side
    gl->glColor3f(0.0f, 0.9750f, 0.0f);
    for (int i = 1; i < 11; i++) gl->glVertex2d((i * 40), down);
    gl->glEnd();

        //Are you on the right side of line? So its green.
        if ((offlineDistance) < 0.0)
        {
            int dots = dotDistance * -1 / 32;

            gl->glPointSize(32.0f);
            gl->glColor3f(0.0f, 0.0f, 0.0f);
            gl->glBegin(GL_POINTS);
            for (int i = 1; i < dots + 1; i++) gl->glVertex2d((i * 40), down);
            gl->glEnd();

            gl->glPointSize(24.0f);
            gl->glColor3f(0.0f, 0.980f, 0.0f);
            gl->glBegin(GL_POINTS);
            for (int i = 0; i < dots; i++) gl->glVertex2d((i * 40 + 40), down);
            gl->glEnd();
            //return;
        }

        else
        {
            int dots = dotDistance / 32;

            gl->glPointSize(32.0f);
            gl->glColor3f(0.0f, 0.0f, 0.0f);
            gl->glBegin(GL_POINTS);
            for (int i = 1; i < dots + 1; i++) gl->glVertex2d((i * -40), down);
            gl->glEnd();

            gl->glPointSize(24.0f);
            gl->glColor3f(0.980f, 0.30f, 0.0f);
            gl->glBegin(GL_POINTS);
            for (int i = 0; i < dots; i++) gl->glVertex2d((i * -40 - 40), down);
            gl->glEnd();
            //return;
        }

    //yellow center dot
    if (dotDistance >= -10 && dotDistance <= 10)
    {
        gl->glPointSize(32.0f);
        gl->glColor3f(0.0f, 0.0f, 0.0f);
        gl->glBegin(GL_POINTS);
        gl->glVertex2d(0, down);
        //gl->glVertex(0, down + 50);
        gl->glEnd();

        gl->glPointSize(24.0f);
        gl->glColor3f(0.980f, 0.98f, 0.0f);
        gl->glBegin(GL_POINTS);
        gl->glVertex2d(0, down);
        //gl->glVertex(0, down + 50);
        gl->glEnd();
    }

    else
    {

        gl->glPointSize(8.0f);
        gl->glColor3f(0.00f, 0.0f, 0.0f);
        gl->glBegin(GL_POINTS);
        gl->glVertex2d(-0, down);
        //gl->glVertex(0, down + 30);
        //gl->glVertex(0, down + 60);
        gl->glEnd();

        //gl->glPointSize(4.0f);
        //gl->glColor(0.9250f, 0.9250f, 0.250f);
        //gl->glBegin(OpenGL.GL_POINTS);
        //gl->glVertex(0, down);
        //gl->glVertex(0, down + 30);
        //gl->glVertex(0, down + 60);
        //gl->glEnd();
    }
}

//void FormGPS::calcFrustum(QOpenGLFunctions_2_1 *gl)
void FormGPS::calcFrustum(const QMatrix4x4 &projection, const QMatrix4x4 &modelview)
{
    //QOpenGLFunctions_1_1 *gl = glContext->versionFunctions<QOpenGLFunctions_1_1>();

    //float proj[16];							// For Grabbing The PROJECTION Matrix
    //float modl[16];							// For Grabbing The MODELVIEW Matrix
    //float clip[16];							// Result Of Concatenating PROJECTION and MODELVIEW
    //float t;											    // Temporary Work Variable

    /*
    gl->glGetFloatv (GL_PROJECTION_MATRIX, proj);		// Grab The Current PROJECTION Matrieprojection
    gl->glGetFloatv(GL_MODELVIEW_MATRIX, modl);		// Grab The Current MODELVIEW Matrix


    // Concatenate (Multiply) The Two Matricies
    clip[0] = modl[0] * proj[0] + modl[1] * proj[4] + modl[2] * proj[8] + modl[3] * proj[12];
    clip[1] = modl[0] * proj[1] + modl[1] * proj[5] + modl[2] * proj[9] + modl[3] * proj[13];
    clip[2] = modl[0] * proj[2] + modl[1] * proj[6] + modl[2] * proj[10] + modl[3] * proj[14];
    clip[3] = modl[0] * proj[3] + modl[1] * proj[7] + modl[2] * proj[11] + modl[3] * proj[15];

    clip[4] = modl[4] * proj[0] + modl[5] * proj[4] + modl[6] * proj[8] + modl[7] * proj[12];
    clip[5] = modl[4] * proj[1] + modl[5] * proj[5] + modl[6] * proj[9] + modl[7] * proj[13];
    clip[6] = modl[4] * proj[2] + modl[5] * proj[6] + modl[6] * proj[10] + modl[7] * proj[14];
    clip[7] = modl[4] * proj[3] + modl[5] * proj[7] + modl[6] * proj[11] + modl[7] * proj[15];

    clip[8] = modl[8] * proj[0] + modl[9] * proj[4] + modl[10] * proj[8] + modl[11] * proj[12];
    clip[9] = modl[8] * proj[1] + modl[9] * proj[5] + modl[10] * proj[9] + modl[11] * proj[13];
    clip[10] = modl[8] * proj[2] + modl[9] * proj[6] + modl[10] * proj[10] + modl[11] * proj[14];
    clip[11] = modl[8] * proj[3] + modl[9] * proj[7] + modl[10] * proj[11] + modl[11] * proj[15];

    clip[12] = modl[12] * proj[0] + modl[13] * proj[4] + modl[14] * proj[8] + modl[15] * proj[12];
    clip[13] = modl[12] * proj[1] + modl[13] * proj[5] + modl[14] * proj[9] + modl[15] * proj[13];
    clip[14] = modl[12] * proj[2] + modl[13] * proj[6] + modl[14] * proj[10] + modl[15] * proj[14];
    clip[15] = modl[12] * proj[3] + modl[13] * proj[7] + modl[14] * proj[11] + modl[15] * proj[15];
    */

    const float *clip;

    clip = (projection * modelview).constData();


    // Extract the RIGHT clipping plane
    frustum[0] = clip[3] - clip[0];
    frustum[1] = clip[7] - clip[4];
    frustum[2] = clip[11] - clip[8];
    frustum[3] = clip[15] - clip[12];

    // Normalize it
    //t = (float)Math.Sqrt(frustum[0] * frustum[0] + frustum[1] * frustum[1] + frustum[2] * frustum[2]);
    //frustum[0] /= t;
    //frustum[1] /= t;
    //frustum[2] /= t;
    //frustum[3] /= t;


    // Extract the LEFT clipping plane
    frustum[4] = clip[3] + clip[0];
    frustum[5] = clip[7] + clip[4];
    frustum[6] = clip[11] + clip[8];
    frustum[7] = clip[15] + clip[12];

    // Normalize it
    //t = (float)Math.Sqrt(frustum[4] * frustum[4] + frustum[5] * frustum[5] + frustum[6] * frustum[6]);
    //frustum[4] /= t;
    //frustum[5] /= t;
    //frustum[6] /= t;
    //frustum[7] /= t;

    // Extract the FAR clipping plane
    frustum[8] = clip[3] - clip[2];
    frustum[9] = clip[7] - clip[6];
    frustum[10] = clip[11] - clip[10];
    frustum[11] = clip[15] - clip[14];

    // Normalize it
    //t = (float)Math.Sqrt(frustum[8] * frustum[8] + frustum[9] * frustum[9] + frustum[10] * frustum[10]);
    //frustum[8] /= t;
    //frustum[9] /= t;
    //frustum[10] /= t;
    //frustum[11] /= t;

    // Extract the NEAR clipping plane.  This is last on purpose (see pointinfrustum() for reason)
    frustum[12] = clip[3] + clip[2];
    frustum[13] = clip[7] + clip[6];
    frustum[14] = clip[11] + clip[10];
    frustum[15] = clip[15] + clip[14];

    // Normalize it
    //t = (float)Math.Sqrt(frustum[12] * frustum[12] + frustum[13] * frustum[13] + frustum[14] * frustum[14]);
    //frustum[12] /= t;
    //frustum[13] /= t;
    //frustum[14] /= t;
    //frustum[15] /= t;

    // Extract the BOTTOM clipping plane
    frustum[16] = clip[3] + clip[1];
    frustum[17] = clip[7] + clip[5];
    frustum[18] = clip[11] + clip[9];
    frustum[19] = clip[15] + clip[13];

    // Normalize it
    //t = (float)Math.Sqrt(frustum[16] * frustum[16] + frustum[17] * frustum[17] + frustum[18] * frustum[18]);
    //frustum[16] /= t;
    //frustum[17] /= t;
    //frustum[18] /= t;
    //frustum[19] /= t;


    // Extract the TOP clipping plane
    frustum[20] = clip[3] - clip[1];
    frustum[21] = clip[7] - clip[5];
    frustum[22] = clip[11] - clip[9];
    frustum[23] = clip[15] - clip[13];

    // Normalize it
    //t = (float)Math.Sqrt(frustum[20] * frustum[20] + frustum[21] * frustum[21] + frustum[22] * frustum[22]);
    //frustum[20] /= t;
    //frustum[21] /= t;
    //frustum[22] /= t;
    //frustum[23] /= t;
}

//take the distance from object and convert to camera data
//TODO, move Projection matrix stuff into here when OpenGL ES migration is complete
void FormGPS::setZoom()
{
    //match grid to cam distance and redo perspective
    if (camera.camSetDistance <= -20000) gridZoom = 2000;
    if (camera.camSetDistance >= -20000 && camera.camSetDistance < -10000) gridZoom =   2000;
    if (camera.camSetDistance >= -10000 && camera.camSetDistance < -5000) gridZoom =    1000;
    if (camera.camSetDistance >= -5000 && camera.camSetDistance < -2000) gridZoom =     503;
    if (camera.camSetDistance >= -2000 && camera.camSetDistance < -1000) gridZoom =     201.2;
    if (camera.camSetDistance >= -1000 && camera.camSetDistance < -500) gridZoom =      100.6;
    if (camera.camSetDistance >= -500 && camera.camSetDistance < -250) gridZoom =       50.3;
    if (camera.camSetDistance >= -250 && camera.camSetDistance < -150) gridZoom =       25.15;
    if (camera.camSetDistance >= -150 && camera.camSetDistance < -50) gridZoom =         10.06;
    if (camera.camSetDistance >= -50 && camera.camSetDistance < -1) gridZoom = 5.03;
    //1.216 2.532


}

void FormGPS::loadGLTextures(QOpenGLFunctions_2_1 *gl)
{
    //QOpenGLFunctions_2_1 *gl = glContext->versionFunctions<QOpenGLFunctions_2_1>();

    QOpenGLTexture *t;

    //  Background
    t = new QOpenGLTexture(QImage(":/images/Landscape.png").mirrored());
    //t->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    //t->setMagnificationFilter(QOpenGLTexture::Linear);

    texture1.append(t); //position 0

    //  Floor
    t = new QOpenGLTexture(QImage(":/images/floor.png"));
    //t->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    //t->setMagnificationFilter(QOpenGLTexture::Linear);
    //t->setWrapMode(QOpenGLTexture::Repeat);

    texture1.append(t); //position 1
    /*
    QImage tex = QGLWidget::convertToGLFormat(QImage(":/images/Landscape.png"));
    gl->glGenTextures(1,&texture[0]);
    gl->glBindTexture(GL_TEXTURE_2D, texture[0]);
    gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    gl->glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

    tex = QGLWidget::convertToGLFormat(QImage(":/images/floor.png").mirrored());
    qDebug() << tex.width() << ", " << tex.height();
    gl->glGenTextures(1,&texture[1]);
    gl->glBindTexture(GL_TEXTURE_2D, texture[1]);
    gl->glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    qDebug() << "textures: " << texture[0] << " " << texture[1];
    */
}
