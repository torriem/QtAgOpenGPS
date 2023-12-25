#include "cworldgrid.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QQuickView>
#include <QThread>
#include <assert.h>
#include "ccamera.h"
#include "aogproperty.h"
#include "glutils.h"
#include "glm.h"

struct SurfaceVertex {
    QVector3D vertex;
    QVector2D textureCoord;
};

CWorldGrid::CWorldGrid()
{
    northingMaxGeo = 300;
    northingMinGeo = -300;
    eastingMaxGeo = 300;
    eastingMinGeo = -300;
}

CWorldGrid::~CWorldGrid() {
    //RAII should destroy our buffers for us?  Not too
    //worried about leaking since this is a singleton that
    //lives for the full lifetime of QtAOG
}

void CWorldGrid::drawFieldSurface(QOpenGLFunctions *gl, const QMatrix4x4 &mvp,
                                  bool isTextureOn, QColor fieldColor,
                                  CCamera &camera)
{
    //We can save a lot of time by keeping this grid buffer on the GPU unless it needs to
    //be altered.
    if (isTextureOn) {
        //adjust bitmap zoom based on cam zoom
        if (camera.zoomValue > 100) Count = 4;
        else if (camera.zoomValue > 80) Count = 8;
        else if (camera.zoomValue > 50) Count = 16;
        else if (camera.zoomValue > 20) Count = 32;
        else if (camera.zoomValue > 10) Count = 64;
        else Count = 80;

        //check to see if we need to regenerate the buffers
        invalidateBuffers();

        if (!fieldBufferCurrent) {
            //regenerate the field surface VBO
            QVector<QVector3D> vertices;

            SurfaceVertex field[] = {
                { QVector3D(eastingMin, northingMax, 0.10),
                  QVector2D(0,0) },
                { QVector3D(eastingMax, northingMax, 0.10),
                  QVector2D(Count, 0.0) },
                { QVector3D(eastingMin, northingMin, 0.10),
                  QVector2D(0,Count) },
                { QVector3D(eastingMax, northingMin, 0.10),
                  QVector2D(Count, Count) }
            };

            if (fieldBuffer.isCreated())
                fieldBuffer.destroy();
            fieldBuffer.create();
            fieldBuffer.bind();
            fieldBuffer.allocate(field, sizeof(SurfaceVertex) * 4);
            fieldBuffer.release();

            fieldBufferCurrent = true;
            lastCount=Count;
        }

        //Now draw the field surface
        //bind field surface texture
        texture[Textures::FLOOR]->bind();
        //qDebug() << texture[Textures::FLOOR] -> width();

        glDrawArraysTexture(gl, mvp, GL_TRIANGLE_STRIP, fieldBuffer, GL_FLOAT, 4, true, fieldColor);

        texture[Textures::FLOOR]->release();

        //TODO If isGeoMap

    }
    else
    {
        GLHelperOneColor gldraw;

        gldraw.append(QVector3D(eastingMin, northingMax, 0.0));
        gldraw.append(QVector3D(eastingMax, northingMax, 0.0));
        gldraw.append(QVector3D(eastingMin, northingMin, 0.0));
        gldraw.append(QVector3D(eastingMax, northingMin, 0.0));

        gldraw.draw(gl,mvp,fieldColor,GL_TRIANGLE_STRIP,1.0f);
    }


    /*
    GLHelperTexture gldraw;
    gldraw.append( { QVector3D(eastingMin, northingMax, 0.0), QVector2D(0,0) } );
    gldraw.append( { QVector3D(eastingMax, northingMax, 0.0), QVector2D(texZoomE, 0) } );
    gldraw.append( { QVector3D(eastingMin, northingMin, 0.0), QVector2D(0,texZoomN) } );
    gldraw.append( { QVector3D(eastingMax, northingMin, 0.0), QVector2D(texZoomE, texZoomN) } );
    gldraw.draw(gl, mvp, Textures::FLOOR, GL_TRIANGLE_STRIP, true, fieldColor);
    */
}

void CWorldGrid::drawWorldGrid(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, double _gridZoom, QColor gridColor)
{
    //draw easting lines and westing lines to produce a grid
    _gridZoom *= 0.5;

    if (_gridZoom != lastGridZoom) { //use epsilon here?
        // if the grid has changed, regenerate the buffer to save
        // us time later.
        QVector<QVector3D> vertices;
        lastGridZoom = _gridZoom;


        for (double num = glm::roundMidAwayFromZero(eastingMin / _gridZoom) * _gridZoom; num < eastingMax; num += _gridZoom)
        {
            if (num < eastingMin) continue;
            vertices.append(QVector3D(num,northingMax, 0.1));
            vertices.append(QVector3D(num,northingMin, 0.1));
        }

        for (double num2 = glm::roundMidAwayFromZero(northingMin / _gridZoom) * _gridZoom; num2 < northingMax; num2 += _gridZoom)
        {
            if (num2 < northingMin) continue;
            vertices.append(QVector3D(eastingMax,num2, 0.1));
            vertices.append(QVector3D(eastingMin,num2, 0.1));
        }

        if (gridBuffer.isCreated())
            gridBuffer.destroy();
        gridBuffer.create();
        gridBuffer.bind();
        gridBuffer.allocate(vertices.data(),vertices.count() * sizeof(QVector3D));
        gridBuffer.release();

        gridBufferCount = vertices.count();
    }
    gl->glLineWidth(1);
    glDrawArraysColor(gl, mvp,
                      GL_LINES, gridColor,
                      gridBuffer,GL_FLOAT,
                      gridBufferCount);
}

void CWorldGrid::checkZoomWorldGrid(double northing, double easting) {
    //hmm, buffers are still redone a lot, maybe caching isn't worth it

    double n = glm::roundMidAwayFromZero(northing / (GridSize / Count * 2)) * (GridSize / Count * 2);
    double e = glm::roundMidAwayFromZero(easting / (GridSize / Count * 2)) * (GridSize / Count * 2);

    northingMax = n + GridSize;
    northingMin = n - GridSize;
    eastingMax = e + GridSize;
    eastingMin = e - GridSize;

    //check to see if we need to regenerate all the buffers for OpenGL
    invalidateBuffers();
}

void CWorldGrid::destroyGLBuffers() {
    //assume valid OpenGL context
    delete fieldShader;
    fieldShader = 0;

    fieldBuffer.destroy();
    gridBuffer.destroy();
}
