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
#include "aogrenderer.h"
#include "cnmea.h"
#include "glm.h"
#include "glutils.h"
#include "qmlutil.h"

#include <QGLWidget>
#include <QQuickView>
#include <QOpenGLFunctions_2_1>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include <assert.h>

void FormGPS::openGLControl_Draw()
{
    QOpenGLContext *glContext = QOpenGLContext::currentContext();
    QOpenGLFunctions *gl = glContext->functions();
    //int width = glContext->surface()->size().width();
    //int height = glContext->surface()->size().height();
    QMatrix4x4 projection;
    QMatrix4x4 modelview;

    int width = qmlItem(qml_root, "openglcontrol")->property("width").toReal();
    int height = qmlItem(qml_root, "openglcontrol")->property("height").toReal();
    gl->glViewport(0,0,width,height);
    //qDebug() << width << height;

    gl->glEnable(GL_POINT_SPRITE);
    gl->glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    //std::cout << "draw routine here." << std::endl;

    //Do stuff that was in the initialized method, since Qt uses OpenGL and may
    //have messed with the matrix stuff and other defaults

    //qml has messed with the state variables; reset them to a sane state.
    //qmlview->resetOpenGLState();
    //  Set the clear color.

    //gl->glClearColor(0.22f, 0.2858f, 0.16f, 1.0f);

    // Set The Blending Function For Translucency
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glDisable(GL_CULL_FACE);
    //gl->glCullFace(GL_BACK);
    gl->glDisable(GL_CULL_FACE);

    //set the camera to right distance
    setZoom();

    //now move the "camera" to the calculated zoom settings
    //I had to move these functions here because if setZoom is called
    //from elsewhere in the GUI (say a button press), there's no GL
    //context to work with.
    projection.setToIdentity();

    //  Create a perspective transformation.
    projection.perspective(fovy, width / (double)height, 1, camDistanceFactor * camera.camSetDistance);


    //double aspect = width / (float)height;
    //double fH = tan( fovy / 360 * M_PI);
    //double fW = fH * aspect;
    //projection.frustum(-fW, fW, -fH, fH, 1, camDistanceFactor * camera.camSetDistance );

    if (isGPSPositionInitialized)
    {

        //  Clear the color and depth buffer.
        gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        modelview.setToIdentity();

        //camera does translations and rotations
        camera.setWorldCam(modelview, vehicle->pivotAxlePos.easting, vehicle->pivotAxlePos.northing, fixHeadingCam);

        //calculate the frustum planes for culling
        calcFrustum(projection*modelview);

        //draw the field ground images
        worldGrid->drawFieldSurface(gl, projection *modelview, *(texture1[1]));

        ////Draw the world grid based on camera position
        gl->glDisable(GL_DEPTH_TEST);
        gl->glDisable(GL_TEXTURE_2D);

        ////if grid is on draw it
        if (isGridOn) worldGrid->drawWorldGrid(gl,projection*modelview,gridZoom);

        //turn on blend for paths
        gl->glEnable(GL_BLEND);

        //section patch color
        QColor sectionColor = QColor(settings.value("display/sectionColor", "#32DCC8").toString());
        sectionColor.setAlpha(160);

        //OpenGL ES does not support wireframe in this way. If we want wireframe,
        //we'll have to do it with LINES
        //if (isDrawPolygons) gl->glPolygonMode(GL_FRONT, GL_LINE);

        //draw patches of sections
        for (int j = 0; j < vehicle->numSuperSection; j++)
        {
            //every time the section turns off and on is a new patch
            int patchCount = vehicle->section[j].patchList.size();

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
                foreach (QSharedPointer<QVector<QVector3D>> triList, vehicle->section[j].patchList)
                {
                    isDraw = false;
                    int count2 = triList->size();
                    for (int i = 0; i < count2; i += 3)
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
                        triBuffer.allocate(triList->data(), count2 * sizeof(QVector3D));
                        triBuffer.release();

                        //TODO: to skip triangles, we can use an index buffer and glDrawElements
                        /*
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
                        triBuffer.allocate(vertices.data(),vertices.count()*sizeof(QVector3D));
                        */
                        glDrawArraysColor(gl,projection*modelview,
                                          GL_TRIANGLE_STRIP, sectionColor,
                                          triBuffer,GL_FLOAT,count2);

                        triBuffer.destroy();
                    }
                }
            }
        }

        //draw contour line if button on
        if (ct->isContourBtnOn) ct->drawContourLine(glContext, modelview, projection);

        // draw the current and reference AB Lines
        else { if (ABLine->isABLineSet || ABLine->isABLineBeingSet) ABLine->drawABLines(glContext, modelview, projection); }

        //draw the flags if there are some

        if (!flagsBufferCurrent) {

            if(flagsBuffer.isCreated())
                flagsBuffer.destroy();
            flagsBuffer.create();
            flagsBuffer.bind();

            QVector<ColorVertex> flagpoints;

            for (int f = 0; f < flagPts.size(); f++)
            {
                QVector4D color;
                if (flagPts[f].color == 0) color = QVector4D(1.0,0,flagPts[f].ID/255.0,1.0);
                if (flagPts[f].color == 1) color = QVector4D(0,1.0,flagPts[f].ID/255.0,1.0);
                if (flagPts[f].color == 2) color = QVector4D(1.0,1.0,flagPts[f].ID/255.0,1.0);
                flagpoints.append( { QVector3D(flagPts[f].easting, flagPts[f].northing, 0),
                                     color } );
            }
            flagsBuffer.allocate(flagpoints.data(),flagPts.size() * sizeof(ColorVertex));
            flagsBuffer.release();

            flagsBufferCurrent = true;
        }

        glDrawArraysColors(gl,projection * modelview,
                           GL_POINTS, flagsBuffer,
                           GL_FLOAT, flagPts.size(), 8.0f);

        if (flagNumberPicked != 0)
        {
            QOpenGLBuffer pickedFlagBuffer;

            pickedFlagBuffer.create();
            pickedFlagBuffer.bind();


            ////draw the box around flag
            QVector4D color(0.980f, 0.0f, 0.980f, 1.0f);

            double offSet = (zoomValue * zoomValue * 0.01);
            ColorVertex boxCoords[5] = {
                { QVector3D(flagPts[flagNumberPicked - 1].easting, flagPts[flagNumberPicked - 1].northing + offSet, 0), color },
                { QVector3D(flagPts[flagNumberPicked - 1].easting - offSet, flagPts[flagNumberPicked - 1].northing, 0), color },
                { QVector3D(flagPts[flagNumberPicked - 1].easting, flagPts[flagNumberPicked - 1].northing - offSet, 0), color },
                { QVector3D(flagPts[flagNumberPicked - 1].easting + offSet, flagPts[flagNumberPicked - 1].northing, 0), color },
                { QVector3D(flagPts[flagNumberPicked - 1].easting, flagPts[flagNumberPicked - 1].northing + offSet, 0), color },
            };

            pickedFlagBuffer.allocate(boxCoords, 5*sizeof(ColorVertex));
            pickedFlagBuffer.release();

            glDrawArraysColors(gl, projection*modelview,
                               GL_LINE_STRIP, pickedFlagBuffer,
                               GL_FLOAT, 5, 4.0f);

            pickedFlagBuffer.destroy();
            pickedFlagBuffer.create();
            pickedFlagBuffer.bind();

            ColorVertex blackDot[1] = {
                { QVector3D(flagPts[flagNumberPicked - 1].easting, flagPts[flagNumberPicked - 1].northing, 0),
                  QVector4D(0.0,0.0,0.0,1.0) } };

            pickedFlagBuffer.allocate(blackDot, sizeof(ColorVertex));
            pickedFlagBuffer.release();

            //draw the flag with a black dot inside
            glDrawArraysColors(gl, projection*modelview,
                               GL_POINTS, pickedFlagBuffer,
                               GL_FLOAT, 1, 4.0f);
            pickedFlagBuffer.destroy();
        }

        //draw the perimter line, returns if no line to draw
        periArea.drawPerimeterLine(glContext,projection*modelview);

        //draw the boundary line
        boundary->drawBoundaryLine(gl, projection*modelview);

        //screen text for debug
        //glDrawText(120, 10, 1, 1, 1, "Courier", 18, " camstep: " + testInt.ToString());
        //glDrawText(120, 40, 1, 1, 1, "Courier", 18, "head: " + Convert.ToString((double)(testDouble)));
        //glDrawText(120, 70, 1, 1, 1, "Courier", 18, " Xe: " + Convert.ToString(Xe));

        //glDrawText(40, 75, 1, 1, 1, "Courier", 16, " SteerCT " + Convert.ToString(ct.steeringAngleCT));
        //glDrawText(40, 90, 1, 1, 1, "Courier", 12, " RadiusCT " + Convert.ToString(ct.radiusCT));
        //glDrawText(40, 105, 1, 0.5f, 1, "Courier", 12, " TrigSetDist(m) " + Convert.ToString(Math.Round(sectionTriggerStepDistance, 2)));
        //glDrawText(40, 120, 1, 0.5, 1, "Courier", 12, " frame msec " + Convert.ToString((int)(frameTime)));

        //draw the vehicle/implement
        vehicle->drawVehicle(glContext,modelview, projection, camera.camSetDistance > -1500);

        //// 2D Ortho --------------------------
        projection.setToIdentity();
        //negative and positive on width, 0 at top to bottom ortho view
        projection.ortho(-(double)width / 2, width / 2, (double)height, 0, -1, 1);

        //  Create the appropriate modelview matrix.
        modelview.setToIdentity();

        if (ui->actionSky_On->isChecked())
        {
            ////draw the background when in 3D
            if (camera.camPitch < -60)
            {
                if ( (lastWidth != width)  || (lastHeight != height)) {
                    lastWidth = width;
                    lastHeight = height;

                    //-10 to -32 (top) is camera pitch range. Set skybox to line up with horizon
                    double hite = (camera.camPitch + 60) / -20 * 0.34;
                    //hite = 0.001;

                    //the background
                    double winLeftPos = -(double)width / 2;
                    double winRightPos = -winLeftPos;

                    //map texture coordinates to model coordinates
                    VertexTexcoord vertices[] = {
                        { QVector3D(winRightPos, 0.0, 0), QVector2D(0,0)}, //top right
                        { QVector3D(winLeftPos, 0.0, 0), QVector2D(1,0)}, //top left
                        { QVector3D(winRightPos, hite*(double)height,0), QVector2D(0,1) }, //bottom right
                        { QVector3D(winLeftPos, hite*(double)height,0), QVector2D(1,1) }, //bottom left
                    };
                    //rebuild sky buffer
                    if (skyBuffer.isCreated())
                        skyBuffer.destroy();
                    skyBuffer.create();
                    skyBuffer.bind();
                    skyBuffer.allocate(vertices,4*sizeof(VertexTexcoord));
                    //skyBuffer.allocate(vertices,4*sizeof(QVector3D));
                    skyBuffer.release();
                }

                texture1[0]->bind();
                glDrawArraysTexture(gl,projection * modelview,
                                    GL_TRIANGLE_STRIP, skyBuffer,
                                    GL_FLOAT,
                                    4);
                texture1[0]->release();
            }
        }

        //LightBar if AB Line is set and turned on or contour
        if (isLightbarOn)
        {
            if (ct->isContourBtnOn)
            {
                if (ct->distanceFromCurrentLine == 32000) ct->distanceFromCurrentLine = 0;

                drawLightBar(width, height, ct->distanceFromCurrentLine * 0.1, modelview, projection);
            } else if (ABLine->isABLineSet || ABLine->isABLineBeingSet) {
                drawLightBar(width, height,
                             ABLine->distanceFromCurrentLine * 0.1,
                             modelview, projection);
            }

        }

        gl->glFlush();//finish openGL commands
        //gl->glPopMatrix();//  Pop the modelview.

        //  back to the projection and pop it, then back to the model view.
        //gl->glMatrixMode(GL_PROJECTION);
        //gl->glPopMatrix();
        //gl->glMatrixMode(GL_MODELVIEW);

        //reset point size
        //gl->glPointSize(1.0f);
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
                    qDebug() << flagNumberPicked;
                    break;
                }
            }
        }
    }
    else
    {
        gl->glClear(GL_COLOR_BUFFER_BIT);
    }
    //qmlview->resetOpenGLState();

    //directly call section lookahead GL stuff from here
    openGLControlBack_Draw();
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
    /*
    delete simpleColorShader;
    simpleColorShader = 0;

    delete interpColorShader;
    interpColorShader = 0;
    */
    //free textures
    foreach(QOpenGLTexture *t, texture1)
    {
        t->destroy();
    }

    //destroy any openGL buffers.
    worldGrid->destroyGLBuffers();
    vehicle->destroyGLBuffers();

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
    QOpenGLFunctions *gl = glContext->functions();
    QMatrix4x4 projection;
    QMatrix4x4 modelview;

    /* use the QML context with an offscreen surface to draw
     * the lookahead triangles
     */
    if (!backFBO ) {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        //TODO: backFBO is leaking... delete it in the destructor?
        //I think context has to be active to delete it...
        backFBO = new QOpenGLFramebufferObject(QSize(400,400),format);
    }

    glContext->makeCurrent(&backSurface);
    backFBO->bind();
    glContext->functions()->glViewport(0,0,400,400);

    //int width = glContext->surface()->size().width();
    //int height = glContext->surface()->size().height();

    gl->glPixelStorei(GL_PACK_ALIGNMENT, 1);

    /* set projection */
    //gl->glMatrixMode(GL_PROJECTION);

    //  Load the identity.
    //gl->glLoadIdentity();
    projection.setToIdentity();

    // change these at your own peril!!!! Very critical
    //  Create a perspective transformation.
    //gl->glPerspective(6.0f, 1, 1, 6000);
    projection.perspective(6.0f,1,1,6000);

    //double fH = tan( 6.0 / 360 * M_PI);
    //double fW = fH;
    //gl->glFrustum(-fW, fW, -fH, fH, 1, 6000 );

    /* end set projection */

    //  Set the modelview matrix.
    //gl->glMatrixMode(GL_MODELVIEW);

    gl->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
    //gl->glLoadIdentity();					// Reset The View
    modelview.setToIdentity();

    //back the camera up
    //gl->glTranslated(0, 0, -390);
    modelview.translate(0, 0, -390);

    //rotate camera so heading matched fix heading in the world
    //gl->glRotated(toDegrees(fixHeadingSection), 0, 0, 1);
    modelview.rotate(toDegrees(vehicle->fixHeadingSection), 0, 0, 1);

    //translate to that spot in the world
    //gl->glTranslated(-toolPos.easting, -toolPos.northing, -fixZ);
    modelview.translate(-vehicle->toolPos.easting, -vehicle->toolPos.northing, -vehicle->fixZ);

    //patch color
    //QColor patchColor(0.0f, 0.5f, 0.0f);
    QColor patchColor = QColor(0,128,0);
    //gl->glColor3f(0.0f, 0.5f, 0.0f);

    //calculate the frustum for the section control window
    calcFrustum(projection* modelview);

    //to draw or not the triangle patch
    bool isDraw;

    //draw patches j= # of sections
    for (int j = 0; j < vehicle->numSuperSection; j++)
    {
        //every time the section turns off and on is a new patch
        int patchCount = vehicle->section[j].patchList.size();

        if (patchCount > 0)
        {
            //for every new chunk of patch
            foreach (QSharedPointer<QVector<QVector3D>> triList, vehicle->section[j].patchList)
            {
                isDraw = false;
                int count2 = triList->size();
                for (int i = 0; i < count2; i+=3)
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

                    triBuffer.allocate(triList->data(), count2 * sizeof(QVector3D));
                    triBuffer.release();

                    //draw the triangles in each triangle strip
                    glDrawArraysColor(gl,projection*modelview,
                                      GL_TRIANGLE_STRIP, patchColor,
                                      triBuffer,GL_FLOAT,count2);

                    triBuffer.destroy();
                }
            }
        }
    }

    //draw boundary line
    boundary->drawBoundaryLineOnBackBuffer(gl, projection*modelview);

    //read the whole block of pixels up to max lookahead, one read only
    //we'll use Qt's QImage function to grab it.
    grnPix = backFBO->toImage().mirrored().convertToFormat(QImage::Format_RGBX8888);
    QImage temp = grnPix.copy(vehicle->rpXPosition, 202, vehicle->rpWidth, 195 /*(int)rpHeight*/);
    memcpy(lookaheadPixels, temp.constBits(), temp.size().width() * temp.size().height() * 4);
    //grnPix = backFBO->toImage().mirrored().convertToFormat(QImage::Format_RGB888);
    grnPix = temp;

    //The remaining code from the original method in the C# code is
    //broken out into a callback in formgps.c called
    //processSectionLookahead().

    glContext->functions()->glFlush();

    //restore QML's context
    backFBO->bindDefault();
    glContext->doneCurrent();
    glContext->makeCurrent(qmlview);
    qmlview->resetOpenGLState();
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

void FormGPS::drawLightBar(double Width, double Height, double offlineDistance,
                           const QMatrix4x4 &modelview, const QMatrix4x4 &projection)
{
    QOpenGLContext *glContext = QOpenGLContext::currentContext();
    QOpenGLFunctions *gl = glContext->functions();
    //QOpenGLFunctions_2_1 *gl = glContext->versionFunctions<QOpenGLFunctions_2_1>();
    double down = 20;

    QOpenGLBuffer dotbuffer;
    QVector<ColorVertex> dots;

    //TODO: Light dots never change, except in size and color. Maybe we could
    //use index buffers to choose which lights we want to draw.

    //  Dot distance is representation of how far from AB Line
    int dotDistance = (int)(offlineDistance);

    if (dotDistance < -320) dotDistance = -320;
    if (dotDistance > 320) dotDistance = 320;

    if (dotDistance < -10) dotDistance -= 30;
    if (dotDistance > 10) dotDistance += 30;

    dotbuffer.create();
    dotbuffer.bind();

    // dot background
    for (int i = -10; i < 0; i++) dots.append({ QVector3D(i*40, down, 0),
                                                QVector4D(0,0,0,1)} ); //gl->glVertex2d((i * 40), down);
    for (int i = 1; i < 11; i++) dots.append({ QVector3D(i*40, down, 0),
                                               QVector4D(0,0,0,1)} ); //gl->glVertex2d((i * 40), down);
    dotbuffer.allocate(dots.constData(), dots.size() * sizeof(ColorVertex));
    dotbuffer.release();
    glDrawArraysColors(gl, projection * modelview,
                      GL_POINTS, dotbuffer,GL_FLOAT,dots.size(), 8.0f);



    dots.clear();

    //red left side
    for (int i = -10; i < 0; i++) dots.append({ QVector3D(i*40, down, 0),   //gl->glVertex2d((i * 40), down);
                                                QVector4D(0.9750f, 0.0f, 0.0f, 1.0f) } );

    //green right side
    for (int i = 1; i < 11; i++) dots.append( { QVector3D(i*40, down, 0), //gl->glVertex2d((i * 40), down);
                                                QVector4D(0.0f, 0.9750f, 0.0f, 1.0f) } );

    dotbuffer.bind();
    dotbuffer.write(0, dots.constData(), dots.size() * sizeof(ColorVertex));
    dotbuffer.release();
    glDrawArraysColors(gl, projection * modelview,
                       GL_POINTS, dotbuffer, GL_FLOAT,
                       dots.size(), 4.0f);

    //Are you on the right side of line? So its green.
    if ((offlineDistance) < 0.0)
    {
        int numDots = dotDistance * -1 / 32;

        dots.clear();
        for (int i = 1; i < numDots + 1; i++) dots.append({ QVector3D(i*40, down, 0), //gl->glVertex2d((i * 40), down);
                                                            QVector4D(0,0,0,1) } );
        dotbuffer.bind();
        dotbuffer.write(0,dots.constData(),dots.size()*sizeof(ColorVertex));
        dotbuffer.release();
        glDrawArraysColors(gl, projection*modelview, GL_POINTS,
                           dotbuffer, GL_FLOAT, dots.size(), 32.0f);

        dots.clear();
        for (int i = 0; i < numDots; i++) dots.append({ QVector3D(i*40+40, down, 0), //gl->glVertex2d((i * 40 + 40), down);
                                                        QVector4D(0.0f, 0.980f, 0.0f,1)} );
        dotbuffer.bind();
        dotbuffer.write(0, dots.constData(), dots.size()*sizeof(ColorVertex));
        dotbuffer.release();
        glDrawArraysColors(gl, projection*modelview, GL_POINTS,
                           dotbuffer, GL_FLOAT, dots.size(), 24.0f);
    }

    else
    {
        int numDots = dotDistance / 32;

        dots.clear();
        for (int i = 1; i < numDots + 1; i++) dots.append({ QVector3D(i*-40, down, 0), //gl->glVertex2d((i * -40), down);
                                                            QVector4D(0,0,0,1) } );
        dotbuffer.bind();
        dotbuffer.write(0,dots.constData(),dots.size()*sizeof(ColorVertex));
        dotbuffer.release();
        glDrawArraysColors(gl, projection*modelview, GL_POINTS,
                           dotbuffer, GL_FLOAT, dots.size(), 32.0f);

        dots.clear();
        for (int i = 0; i < numDots; i++) dots.append({ QVector3D(i*-40-40, down, 0), //gl->glVertex2d((i * -40 - 40), down);
                                                        QVector4D(0.980f, 0.30f, 0.0f,1)} );
        dotbuffer.bind();
        dotbuffer.write(0, dots.constData(), dots.size()*sizeof(ColorVertex));
        dotbuffer.release();
        glDrawArraysColors(gl, projection*modelview, GL_POINTS,
                           dotbuffer, GL_FLOAT, dots.size(), 24.0f);
    }

    //yellow center dot
    ColorVertex p = { QVector3D(0, down, 0),
                      QVector4D(0,0,0,1) };
    dotbuffer.bind();
    dotbuffer.write(0,&p,sizeof(ColorVertex));
    dotbuffer.release();

    if (dotDistance >= -10 && dotDistance <= 10)
    {
       glDrawArraysColors(gl, projection * modelview, GL_POINTS,
                           dotbuffer,GL_FLOAT,1,32.0f);

        p.color = QVector4D(0.980f, 0.98f, 0.0f, 1.0f);
        dotbuffer.bind();
        dotbuffer.write(0,&p,sizeof(ColorVertex));
        dotbuffer.release();
        glDrawArraysColors(gl, projection * modelview, GL_POINTS,
                           dotbuffer,GL_FLOAT,1,24.0f);

    }

    else
    {
        glDrawArraysColors(gl, projection * modelview, GL_POINTS,
                           dotbuffer,GL_FLOAT,1,8.0f);
    }

    dotbuffer.destroy();
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

void FormGPS::loadGLTextures()
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
    t->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    t->setMagnificationFilter(QOpenGLTexture::Linear);
    t->setWrapMode(QOpenGLTexture::Repeat);

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
