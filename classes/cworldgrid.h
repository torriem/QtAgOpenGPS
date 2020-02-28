#ifndef CWORLDGRID_H
#define CWORLDGRID_H

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
//#include <QOpenGLShaderProgram>

class QOpenGLContext;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

class CWorldGrid
{
private:
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

    const double texZoomE = 20;
    const double texZoomN = 20;

    CWorldGrid();
    ~CWorldGrid();
    void drawFieldSurface(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, QColor fieldColor);
    void drawWorldGrid(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, double _gridZoom);
    void createWorldGrid(double northing, double easting);
    void checkZoomWorldGrid(double northing, double easting);
    void destroyGLBuffers();

};

#endif // CWORLDGRID_H
