#ifndef CWORLDGRID_H
#define CWORLDGRID_H

#include <QMatrix4x4>
#include <QOpenGLBuffer>
//#include <QOpenGLShaderProgram>

class FormGPS;
class QOpenGLContext;
class QOpenGLFunctions_2_1;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

class CWorldGrid
{
private:
    FormGPS *mf;

    QOpenGLShaderProgram *fieldShader=0;
    QOpenGLBuffer fieldBuffer;
    QOpenGLBuffer gridBuffer;
    int gridBufferCount;

    bool fieldBufferCurrent = false;
    double lastGridZoom = -1;

    inline void invalidateBuffers() {
        lastGridZoom = -1;
        fieldBufferCurrent = false;
    }

public:
    //Z (Y)
    double northingMax = 0;
    double northingMin = 0;

    double eastingMax = 0;
    double eastingMin = 0;

    const double texZoom = 20;

    CWorldGrid(FormGPS *mf);
    void drawFieldSurface(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);
    void drawWorldGrid(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, double _gridZoom);
    void createWorldGrid(double northing, double easting);
    void checkZoomWorldGrid(double northing, double easting);

};

#endif // CWORLDGRID_H
