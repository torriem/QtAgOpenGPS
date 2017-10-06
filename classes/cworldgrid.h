#ifndef CWORLDGRID_H
#define CWORLDGRID_H

class FormGPS;

class CWorldGrid
{
public:
    //Z (Y)
    double northingMax;
    double northingMin;

    double eastingMax;
    double eastingMin;

    const double texZoom = 20;

    CWorldGrid();
    void drawFieldSurface(FormGPS *mf);
    void drawWorldGrid(FormGPS *mf,double _gridZoom);
    void createWorldGrid(double northing, double easting);
    void checkZoomWorldGrid(double northing, double easting);

};

#endif // CWORLDGRID_H
