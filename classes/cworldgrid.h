#ifndef CWORLDGRID_H
#define CWORLDGRID_H

#include <QObject>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
//#include <QOpenGLShaderProgram>

class QOpenGLContext;
class QOpenGLFunctions;
class QOpenGLShaderProgram;
class CCamera;

class CWorldGrid: public QObject
{
    Q_OBJECT
private:
    QOpenGLShaderProgram *fieldShader=0;
    QOpenGLBuffer fieldBuffer;
    QOpenGLBuffer rateMapBuffer;
    QOpenGLBuffer gridBuffer;
    int gridBufferCount;

    bool fieldBufferCurrent = false;
    bool rateMapBufferCurrent = false;
    double lastGridZoom = -1;
    double lastCount = -1;
    double lastNorthingMax = 0;
    double lastNorthingMin = 0;
    double lastEastingMax = 0;
    double lastEastingMin = 0;


    inline void invalidateBuffers() {
        if ((lastEastingMax != eastingMax) ||
            (lastEastingMin != eastingMin) ||
            (lastNorthingMax != northingMax) ||
            (lastNorthingMin != northingMin) ||
            (lastCount != Count) ) {
        lastGridZoom = -1;
        fieldBufferCurrent = false;
        rateMapBufferCurrent = false;
        lastCount = -1;
        }
    }

public:
    //Y
    double northingMax = 0;
    double northingMin = 0;

    //X
    double eastingMax = 0;
    double eastingMin = 0;

    //Y
    double northingMaxGeo;

    double northingMinGeo;

    //X
    double eastingMaxGeo;

    double eastingMinGeo;

    //Y
    double northingMaxRate;

    double northingMinRate;

    //X
    double eastingMaxRate;

    double eastingMinRate;

    double GridSize = 6000;
    double Count = 40;
    bool isGeoMap = false;
    bool isRateMap = false, isRateTrigger = false;
    int numRateChannels = 1;

    double gridRotation = 0.0;

    explicit CWorldGrid(QObject *parent = 0);
    ~CWorldGrid();
    void DrawFieldSurface(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, bool isTextureOn, QColor fieldColor, CCamera &camera);
    void DrawWorldGrid(QOpenGLFunctions *gl, QMatrix4x4 modelview, QMatrix4x4 projection, double _gridZoom, QColor gridColor);
    void destroyGLBuffers();

public slots:
    void checkZoomWorldGrid(double northing, double easting);

};

#endif // CWORLDGRID_H
