#ifndef CWORLDGRID_H
#define CWORLDGRID_H

class FormGPS;
class QOpenGLContext;

class CWorldGrid
{
private:
    FormGPS *mf;
public:
    //Z (Y)
    double northingMax;
    double northingMin;

    double eastingMax;
    double eastingMin;

    const double texZoom = 20;

    CWorldGrid(FormGPS *mf);
    void drawFieldSurface(QOpenGLContext *c);
    void drawWorldGrid(QOpenGLContext *c, double _gridZoom);
    void createWorldGrid(double northing, double easting);
    void checkZoomWorldGrid(double northing, double easting);

};

#endif // CWORLDGRID_H
