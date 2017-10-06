#include <QtOpenGL>
#include "ui_formgps.h"
#include "formgps.h"
#include "csection.h"
#include "cvehicle.h"
#include "cworldgrid.h"
#include "ccontour.h"
#include "cabline.h"
#include "cperimeter.h"

void FormGPS::openGLControl_Draw()
{
    //Do stuff that was initialized since Qt uses OpenGL and may have messed
    //with the matrix stuff

    //  Set the clear color.
    glClearColor(0.22f, 0.2858f, 0.16f, 1.0f);

    // Set The Blending Function For Translucency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //set the camera to right distance
    setZoom();


    if (isGPSPositionInitialized)
    {

        //  Clear the color and depth buffer.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        //camera does translations and rotations
        camera.setWorldCam(pivotAxlePos.easting, pivotAxlePos.northing, fixHeadingCam);

        //calculate the frustum planes for culling
        calcFrustum();

        //draw the field ground images
        worldGrid->drawFieldSurface();

        ////Draw the world grid based on camera position
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_2D);

        ////if grid is on draw it
        if (isGridOn) worldGrid->drawWorldGrid(gridZoom);

        //turn on blend for paths
        glEnable(GL_BLEND);

        //section patch color
        glColor4b(redSections, grnSections, bluSections, 160);
        if (isDrawPolygons) glPolygonMode(GL_FRONT, GL_LINE);

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
                        glBegin(GL_TRIANGLE_STRIP);
                        count2 = triList->size();

                        //if large enough patch and camera zoomed out, fake mipmap the patches, skip triangles
                        if (count2 >= (mipmap + 2))
                        {
                            int step = mipmap;
                            for (int i = 0; i < count2; i += step)
                            {
                                glVertex3d((*triList)[i].easting, (*triList)[i].northing, 0); i++;
                                glVertex3d((*triList)[i].easting, (*triList)[i].northing, 0); i++;

                                //too small to mipmap it
                                if (count2 - i <= (mipmap + 2)) step = 0;
                            }
                        }

                        else { for (int i = 0; i < count2; i++)  glVertex3d((*triList)[i].easting, (*triList)[i].northing, 0); }
                        glEnd();
                    }
                }
            }
        }


        glPolygonMode(GL_FRONT, GL_FILL);
        glColor3f(1, 1, 1);

        //draw contour line if button on
        if (ct->isContourBtnOn) ct->drawContourLine();

        // draw the current and reference AB Lines
        else { if (ABLine->isABLineSet || ABLine->isABLineBeingSet) ABLine->drawABLines(); }

        //draw the flags if there are some
        int flagCnt = flagPts.size();
        if (flagCnt > 0)
        {
            for (int f = 0; f < flagCnt; f++)
            {
                glPointSize(8.0f);
                glBegin(GL_POINTS);
                if (flagPts[f].color == 0) glColor3b(255, 0, flagPts[f].ID);
                if (flagPts[f].color == 1) glColor3b(0, 255, flagPts[f].ID);
                if (flagPts[f].color == 2) glColor3b(255, 255, flagPts[f].ID);
                glVertex3d(flagPts[f].easting, flagPts[f].northing, 0);
                glEnd();
            }

            if (flagNumberPicked != 0)
            {
                ////draw the box around flag
                glLineWidth(4);
                glColor3f(0.980f, 0.0f, 0.980f);
                glBegin(GL_LINE_STRIP);

                double offSet = (zoomValue * zoomValue * 0.01);
                glVertex3d(flagPts[flagNumberPicked - 1].easting, flagPts[flagNumberPicked - 1].northing + offSet, 0);
                glVertex3d(flagPts[flagNumberPicked - 1].easting - offSet, flagPts[flagNumberPicked - 1].northing, 0);
                glVertex3d(flagPts[flagNumberPicked - 1].easting, flagPts[flagNumberPicked - 1].northing - offSet, 0);
                glVertex3d(flagPts[flagNumberPicked - 1].easting + offSet, flagPts[flagNumberPicked - 1].northing, 0);
                glVertex3d(flagPts[flagNumberPicked - 1].easting, flagPts[flagNumberPicked - 1].northing + offSet, 0);

                glEnd();

                //draw the flag with a black dot inside
                glPointSize(4.0f);
                glColor3f(0, 0, 0);
                glBegin(GL_POINTS);
                glVertex3d(flagPts[flagNumberPicked - 1].easting, flagPts[flagNumberPicked - 1].northing, 0);
                glEnd();
            }
        }

        //draw the perimter line, returns if no line to draw
        periArea.drawPerimeterLine();

        //draw the boundary line
        boundary->drawBoundaryLine();

        //screen text for debug
        //glDrawText(120, 10, 1, 1, 1, "Courier", 18, " camstep: " + testInt.ToString());
        //glDrawText(120, 40, 1, 1, 1, "Courier", 18, "head: " + Convert.ToString((double)(testDouble)));
        //glDrawText(120, 70, 1, 1, 1, "Courier", 18, " Xe: " + Convert.ToString(Xe));

        //glDrawText(40, 75, 1, 1, 1, "Courier", 16, " SteerCT " + Convert.ToString(ct.steeringAngleCT));
        //glDrawText(40, 90, 1, 1, 1, "Courier", 12, " RadiusCT " + Convert.ToString(ct.radiusCT));
        //glDrawText(40, 105, 1, 0.5f, 1, "Courier", 12, " TrigSetDist(m) " + Convert.ToString(Math.Round(sectionTriggerStepDistance, 2)));
        //glDrawText(40, 120, 1, 0.5, 1, "Courier", 12, " frame msec " + Convert.ToString((int)(frameTime)));

        //draw the vehicle/implement
        vehicle->drawVehicle();

        //Back to normal
        glColor3f(0.98f, 0.98f, 0.98f);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        //// 2D Ortho --------------------------
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        //negative and positive on width, 0 at top to bottom ortho view
        //should this be the width of the opengl control?
        glOrtho2D(-(double)width() / 2, (double)width() / 2, (double)height(), 0);

        //  Create the appropriate modelview matrix.
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        if (ui->actionGrid_On->isChecked())

                //skyToolStripMenu.Checked)
        {
            ////draw the background when in 3D
            if (camera.camPitch < -60)
            {
                //-10 to -32 (top) is camera pitch range. Set skybox to line up with horizon
                double hite = (camera.camPitch + 60) / -20 * 0.34;
                //hite = 0.001;

                //the background
                double winLeftPos = -(double)width() / 2;
                double winRightPos = -winLeftPos;
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texture[0]);		// Select Our Texture

                glBegin(GL_TRIANGLE_STRIP);				// Build Quad From A Triangle Strip
                glTexCoord2i(0, 0); glVertex2d(winRightPos, 0.0); // Top Right
                glTexCoord2i(1, 0); glVertex2d(winLeftPos, 0.0); // Top Left
                glTexCoord2i(0, 1); glVertex2d(winRightPos, hite * (double)height()); // Bottom Right
                glTexCoord2i(1, 1); glVertex2d(winLeftPos, hite * (double)height()); // Bottom Left
                glEnd();						// Done Building Triangle Strip

                //disable, straight color
                glDisable(GL_TEXTURE_2D);
            }
        }

        //LightBar if AB Line is set and turned on or contour
        if (isLightbarOn)
        {
            if (ct->isContourBtnOn)
            {
                QString dist;
                //turn on distance widget
                txtDistanceOffABLine.Visible = true;
                //lblDelta.Visible = true;
                if (ct->distanceFromCurrentLine == 32000) ct->distanceFromCurrentLine = 0;

                drawLightBar(ui->openGLControl->width(), ui->openGLControl->height(), ct->distanceFromCurrentLine * 0.1);
                if ((ct->distanceFromCurrentLine) < 0.0)
                {
                    txtDistanceOffABLine.ForeColor = Color.Green;

                    if (isMetric) dist = QString((int)fabs(ct->distanceFromCurrentLine * 0.1)) + " " + QChar(0x2192);
                    else dist = QString((int)fabs(ct->distanceFromCurrentLine / 2.54 * 0.1)) + " " + QChar(0x2192);
                    txtDistanceOffABLine.Text = dist;
                }

                else
                {
                    txtDistanceOffABLine.ForeColor = Color.Red;
                    if (isMetric) dist = QString("") + QChar(0x2190) + " " + ((int)fabs(ct->distanceFromCurrentLine * 0.1));
                    else dist = QString("") + QChar(0x2190)+ " " + ((int)fabs(ct->distanceFromCurrentLine / 2.54 * 0.1));
                    txtDistanceOffABLine.Text = dist;
                }

                //if (guidanceLineHeadingDelta < 0) lblDelta.ForeColor = Color.Red;
                //else lblDelta.ForeColor = Color.Green;

                if (guidanceLineDistanceOff == 32020 || guidanceLineDistanceOff == 32000)
                    btnAutoSteer.Text = "\u2715";
                else btnAutoSteer.Text = "\u2713";
            }

            else
            {
                if (ABLine->isABLineSet || ABLine->isABLineBeingSet)
                {
                    QString dist;

                    txtDistanceOffABLine.Visible = true;
                    //lblDelta.Visible = true;
                    drawLightBar(ui->openGLControl->width(),
                                 ui->openGLControl->height(),
                                 ABLine->distanceFromCurrentLine * 0.1);
                    if ((ABLine->distanceFromCurrentLine) < 0.0)
                    {
                        // --->
                        txtDistanceOffABLine.ForeColor = Color.Green;
                        if (isMetric) dist = QString((int)fabs(ABLine->distanceFromCurrentLine * 0.1)) + " \u21D2";
                        else dist = QString((int)fabs(ABLine->distanceFromCurrentLine / 2.54 * 0.1)) + " \u21D2";
                        txtDistanceOffABLine.Text = dist;
                    }

                    else
                    {
                        // <----
                        txtDistanceOffABLine.ForeColor = Color.Red;
                        if (isMetric) dist = "\u21D0 " + QString((int)fabs(ABLine->distanceFromCurrentLine * 0.1));
                        else dist = "\u21D0 " + QString((int)fabs(ABLine->distanceFromCurrentLine / 2.54 * 0.1));
                        txtDistanceOffABLine.Text = dist;
                    }

                    //if (guidanceLineHeadingDelta < 0) lblDelta.ForeColor = Color.Red;
                    //else lblDelta.ForeColor = Color.Green;
                    if (guidanceLineDistanceOff == 32020 || guidanceLineDistanceOff == 32000)
                        btnAutoSteer.Text = "\u2715";
                    else btnAutoSteer.Text = "\u2713";
                }
            }

            //AB line is not set so turn off numbers
            if (!ABLine->isABLineSet && !ABLine->isABLineBeingSet && !ct->isContourBtnOn)
            {
                txtDistanceOffABLine.Visible = false;
                //lblDelta.Visible = false;
                btnAutoSteer.Text = "-";
            }
        }

        else
        {
            txtDistanceOffABLine.Visible = false;
            //lblDelta.Visible = false;
            btnAutoSteer.Text = "-";
        }

        glFlush();//finish openGL commands
        glPopMatrix();//  Pop the modelview.

        //  back to the projection and pop it, then back to the model view.
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        //reset point size
        glPointSize(1.0f);
        glFlush();

        if (leftMouseDownOnOpenGL)
        {
            leftMouseDownOnOpenGL = false;
            uchar data1[192];

            //scan the center of click and a set of square points around
            glReadPixels(mouseX - 4, mouseY - 4, 8, 8, GL_RGB, GL_UNSIGNED_BYTE, data1);

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

        //draw the section control window off screen buffer
        //openGLControlBack.DoRender();
        //maybe call the openGLControlBack_Draw() directly from here???
    }
}
