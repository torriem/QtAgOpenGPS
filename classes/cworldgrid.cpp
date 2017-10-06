#include "cworldgrid.h"
#include <QtOpenGL>
#include "formgps.h"

CWorldGrid::CWorldGrid(FormGPS *mf)
    :mf(mf)
{

}

//TODO: pass in color and texture to draw functions instead of passing in the
// entire FormGPS object

void CWorldGrid::drawFieldSurface() {
    // Enable Texture Mapping and set color to white
    glEnable(GL_TEXTURE_2D);
    glColor3b(mf->redField, mf->grnField, mf->bluField);

    //the floor
    glBindTexture(GL_TEXTURE_2D, mf->texture[1]);	// Select Our Texture
    glBegin(GL_TRIANGLE_STRIP);				            // Build Quad From A Triangle Strip
    glTexCoord2d(0, 0);
    glVertex3d(eastingMin, northingMin, 0.0);                // Top Right
    glTexCoord2d(texZoom, 0);
    glVertex3d(eastingMax, northingMin, 0.0);               // Top Left
    glTexCoord2d(0, texZoom);
    glVertex3d(eastingMin, northingMax, 0.0);               // Bottom Right
    glTexCoord2d(texZoom, texZoom);
    glVertex3d(eastingMax, northingMax, 0.0);              // Bottom Left
    glEnd();						// Done Building Triangle Strip
    glDisable(GL_TEXTURE_2D);
}

void CWorldGrid::drawWorldGrid(double _gridZoom) {
    //draw easting lines and westing lines to produce a grid

    glColor3b(mf->redField, mf->grnField, mf->bluField);
    glBegin(GL_LINES);
    for (double x = eastingMin; x < eastingMax; x += _gridZoom)
    {
        //the x lines
        glVertex3d(x, northingMax, 0.1 );
        glVertex3d(x, northingMin, 0.1);
    }

    for (double x = northingMin; x < northingMax; x += _gridZoom)
    {
        //the z lines
        glVertex3d(eastingMax, x, 0.1 );
        glVertex3d(eastingMin, x, 0.1 );
    }
    glEnd();
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
