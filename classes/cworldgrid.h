#ifndef CWORLDGRID_H
#define CWORLDGRID_H

class FormGPS;
class QOpenGLContext;
class QOpenGLFunctions_2_1;

class CWorldGrid
{
private:
    FormGPS *mf;
public:
    //Z (Y)
    double northingMax = 0;
    double northingMin = 0;

    double eastingMax = 0;
    double eastingMin = 0;

    const double texZoom = 20;

    CWorldGrid(FormGPS *mf);
    void drawFieldSurface(QOpenGLFunctions_2_1 *gl);
    void drawWorldGrid(QOpenGLFunctions_2_1 *gl, double _gridZoom);
    void createWorldGrid(double northing, double easting);
    void checkZoomWorldGrid(double northing, double easting);

};

#endif // CWORLDGRID_H
