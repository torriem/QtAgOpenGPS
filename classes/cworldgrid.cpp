#include "cworldgrid.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_2_1>
#include <QQuickView>
#include <QThread>
#include <assert.h>
#include "aogsettings.h"
#include "glutils.h"

struct SurfaceVertex {
    QVector3D vertex;
    QVector2D textureCoord;
};

CWorldGrid::CWorldGrid()
{

}

CWorldGrid::~CWorldGrid() {
    //get opengl context for main qml view
    //mf->qmlview->openglContext()->currentContext()->makeCurrent
    //fieldBuffer.destroy();
    //gridBuffer.destroy();
}

void CWorldGrid::drawFieldSurface(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, QOpenGLTexture &texture)
{
    QSettings settings;
    //QOpenGLFunctions_2_1 *gl = glContext->versionFunctions<QOpenGLFunctions_2_1>();

    if (!fieldBufferCurrent) {
        //regenerate the field surface VBO
        QVector<QVector3D> vertices;

        SurfaceVertex field[] = {
            { QVector3D(eastingMin, northingMin, 0.0),
              QVector2D(0,0) },
            { QVector3D(eastingMax, northingMin, 0.0),
              QVector2D(texZoom, 0) },
            { QVector3D(eastingMin, northingMax, 0.0),
              QVector2D(0,texZoom) },
            { QVector3D(eastingMax, northingMax, 0.0),
              QVector2D(texZoom, texZoom) }
        };

        if (fieldBuffer.isCreated())
            fieldBuffer.destroy();
   fieldBuffer.create();
        fieldBuffer.bind();
        fieldBuffer.allocate(field, sizeof(SurfaceVertex) * 4);
        fieldBuffer.release();

        fieldBufferCurrent = true;
    }

    // Enable Texture Mapping and set color to white

    QColor fieldColor = QColor(settings.value("display/fieldColor", "#82781E").toString());

    if (!fieldShader) {
        //have to dynamically create it because we're in a different thread now
        //than when the constructor ran. QThread will destroy it for us when
        //we're done.
        fieldShader = new QOpenGLShaderProgram(QThread::currentThread());
        assert(fieldShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/colortex_vshader.vsh"));
        assert(fieldShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/colortex_fshader.fsh"));
        assert(fieldShader->link());
    }

    //bind field surface texture
    texture.bind();

    //bind and set up the shader
    fieldShader->bind();
    fieldShader->setUniformValue("color", fieldColor);
    fieldShader->setUniformValue("texture", 0); //magic number texture 0?
    fieldShader->setUniformValue("mvpMatrix", mvp);
    fieldShader->setUniformValue("useColor", true);

    fieldBuffer.bind();
    fieldShader->enableAttributeArray("vertex");
    fieldShader->enableAttributeArray("texcoord_src");

    //TODO move these shader and drawing routines to glutils
    gl->glVertexAttribPointer(fieldShader->attributeLocation("vertex"),
                              3, //3D vertices
                              GL_FLOAT, GL_FALSE,
                              5 * sizeof(float),
                              0 //vertex comes first
                             );

    gl->glVertexAttribPointer(fieldShader->attributeLocation("texcoord_src"),
                              2, //2D texture coordinate
                              GL_FLOAT, GL_FALSE,
                              5 * sizeof(float),
                              ((float *)0) + 3 //comes after the vertex
                             );

    gl->glDrawArrays(GL_TRIANGLE_STRIP,0, 4); //A quad from triangles

    texture.release();
    fieldBuffer.release();
    fieldShader->release();

}

void CWorldGrid::drawWorldGrid(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, double _gridZoom)
{
    //QOpenGLFunctions_2_1 *gl = glContext->versionFunctions<QOpenGLFunctions_2_1>();
    //draw easting lines and westing lines to produce a grid
    QSettings settings;

    if (_gridZoom != lastGridZoom) { //use epsilon here?
        QVector<QVector3D> vertices;
        lastGridZoom = _gridZoom;

        for (double x = eastingMin; x < eastingMax; x += lastGridZoom)
        {
            //the x lines
            //gl->glVertex3d(x, northingMax, 0.1 );
            vertices.append(QVector3D(x,northingMax, 0.1));

            //gl->glVertex3d(x, northingMin, 0.1);
            vertices.append(QVector3D(x,northingMin, 0.1));
        }

        for (double x = northingMin; x < northingMax; x += lastGridZoom)
        {
            //the z lines
            //gl->glVertex3d(eastingMax, x, 0.1 );
            vertices.append(QVector3D(eastingMax,x, 0.1));

            //gl->glVertex3d(eastingMin, x, 0.1 );
            vertices.append(QVector3D(eastingMin,x, 0.1));
        }

        if (gridBuffer.isCreated())
            gridBuffer.destroy();
        gridBuffer.create();
        gridBuffer.bind();
        gridBuffer.allocate(vertices.data(),vertices.count() * sizeof(QVector3D));
        gridBuffer.release();

        gridBufferCount = vertices.count();

    }

    QColor fieldColor = QColor(settings.value("display/fieldColor", "#82781E").toString());

    glDrawArraysColor(gl, mvp,
                      GL_LINES, fieldColor,
                      gridBuffer,GL_FLOAT,
                      gridBufferCount);
}

void CWorldGrid::createWorldGrid(double northing, double easting) {
    //draw a grid 5 km each direction away from initial fix
     northingMax = northing + 4000;
     northingMin = northing - 4000;

     eastingMax = easting + 4000;
     eastingMin = easting - 4000;

     //reconstruct VBOs
     fieldBufferCurrent = false;
     lastGridZoom = -1;
}


void CWorldGrid::checkZoomWorldGrid(double northing, double easting) {
    //make sure the grid extends far enough away as you move along
    //just keep it growing as you continue to move in a direction - forever.

    //hmm, buffers are still redone a lot.

    if ((northingMax - northing) < 1500) { northingMax = northing + 2000; invalidateBuffers(); }
    if ((northing - northingMin) < 1500) { northingMin = northing - 2000; invalidateBuffers(); }
    if ((eastingMax - easting) < 1500)  { eastingMax = easting + 2000; invalidateBuffers(); }
    if ((easting - eastingMin) < 1500) { eastingMin = easting - 2000; invalidateBuffers(); }
}

void CWorldGrid::destroyGLBuffers() {
    //assume valid OpenGL context
    delete fieldShader;
    fieldShader = 0;

    fieldBuffer.destroy();
    gridBuffer.destroy();
}
