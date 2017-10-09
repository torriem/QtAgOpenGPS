#include "cworldgrid.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions_2_1>
#include "formgps.h"

CWorldGrid::CWorldGrid(FormGPS *mf)
    :mf(mf)
{

}

//TODO: pass in color and texture to draw functions instead of passing in the
// entire FormGPS object

void CWorldGrid::drawFieldSurface(QOpenGLFunctions_2_1 *gl)
{
    //QOpenGLFunctions_2_1 *gl = glContext->versionFunctions<QOpenGLFunctions_2_1>();
    gl->initializeOpenGLFunctions();//should already be initialized
    // Enable Texture Mapping and set color to white
    gl->glEnable(GL_TEXTURE_2D);
    gl->glColor3b(mf->redField, mf->grnField, mf->bluField);

    //the floor
    gl->glBindTexture(GL_TEXTURE_2D, mf->texture[1]);	// Select Our Texture
    //mf->texture[1]->bind();
    gl->glBegin(GL_TRIANGLE_STRIP);				            // Build Quad From A Triangle Strip
    gl->glTexCoord2d(0, 0);
    gl->glVertex3d(eastingMin, northingMin, 0.0);                // Top Right
    gl->glTexCoord2d(texZoom, 0);
    gl->glVertex3d(eastingMax, northingMin, 0.0);               // Top Left
    gl->glTexCoord2d(0, texZoom);
    gl->glVertex3d(eastingMin, northingMax, 0.0);               // Bottom Right
    gl->glTexCoord2d(texZoom, texZoom);
    gl->glVertex3d(eastingMax, northingMax, 0.0);              // Bottom Left
    gl->glEnd();						// Done Building Triangle Strip
    gl->glDisable(GL_TEXTURE_2D);
}

void CWorldGrid::drawWorldGrid(QOpenGLFunctions_2_1 *gl, double _gridZoom)
{
    //QOpenGLFunctions_2_1 *gl = glContext->versionFunctions<QOpenGLFunctions_2_1>();
    //draw easting lines and westing lines to produce a grid

    gl->glColor3b(mf->redField, mf->grnField, mf->bluField);
    gl->glBegin(GL_LINES);
    for (double x = eastingMin; x < eastingMax; x += _gridZoom)
    {
        //the x lines
        gl->glVertex3d(x, northingMax, 0.1 );
        gl->glVertex3d(x, northingMin, 0.1);
    }

    for (double x = northingMin; x < northingMax; x += _gridZoom)
    {
        //the z lines
        gl->glVertex3d(eastingMax, x, 0.1 );
        gl->glVertex3d(eastingMin, x, 0.1 );
    }
    gl->glEnd();
}

void CWorldGrid::createWorldGrid(double northing, double easting) {
    //draw a grid 5 km each direction away from initial fix
     northingMax = northing + 4000;
     northingMin = northing - 4000;

     eastingMax = easting + 4000;
     eastingMin = easting - 4000;
}


void CWorldGrid::checkZoomWorldGrid(double northing, double easting) {
    //make sure the grid extends far enough away as you move along
    //just keep it growing as you continue to move in a direction - forever.
    if ((northingMax - northing) < 1500) northingMax = northing + 2000;
    if ((northing - northingMin) < 1500) northingMin = northing - 2000;
    if ((eastingMax - easting) < 1500) eastingMax = easting + 2000;
    if ((easting - eastingMin) < 1500) eastingMin = easting - 2000;
}
