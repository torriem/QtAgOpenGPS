#include "glutils.h"
#include <QThread>
#include <QOpenGLShaderProgram>
#include <assert.h>

//module-level symbols
QOpenGLShaderProgram *simpleColorShader = 0;
QOpenGLShaderProgram *texShader = 0;
QOpenGLShaderProgram *interpColorShader = 0;

void initializeShaders() {
    //GL context must be bound by caller, and this must be called from
    //a QThread context

    //All shader memory will be managed by the current QThread
    if (!simpleColorShader) {
        simpleColorShader = new QOpenGLShaderProgram(QThread::currentThread()); //memory managed by Qt
        assert(simpleColorShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/color_vshader.vsh"));
        assert(simpleColorShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/color_fshader.fsh"));
        assert(simpleColorShader->link());
    }
    if (!texShader) {
        texShader = new QOpenGLShaderProgram(QThread::currentThread()); //memory managed by Qt
        assert(texShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/colortex_vshader.vsh"));
        assert(texShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/colortex_fshader.fsh"));
        assert(texShader->link());
    }
    if (!interpColorShader) {
        interpColorShader = new QOpenGLShaderProgram(QThread::currentThread()); //memory managed by Qt
        assert(interpColorShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/colors_vshader.vsh"));
        assert(interpColorShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/colors_fshader.fsh"));
        assert(interpColorShader->link());
    }
}

void destroyShaders() {
    //OpenGL context must be bound by caller.

    if(simpleColorShader) {
        delete simpleColorShader;
        simpleColorShader = 0;
    }

    if(texShader) {
        delete texShader;
        texShader = 0;
    }

    if(interpColorShader) {
        delete interpColorShader;
        interpColorShader = 0;
    }
}

void glDrawArraysColor(QOpenGLFunctions *gl,
                       QMatrix4x4 mvp,
                       GLenum operation,
                       QColor color,
                       QOpenGLBuffer &vertexBuffer,
                       GLenum GL_type,
                       int count,
                       float pointSize)
{
    //bind shader
    assert(simpleColorShader->bind());
    //set color
    simpleColorShader->setUniformValue("color", color);
    //set mvp matrix
    simpleColorShader->setUniformValue("mvpMatrix", mvp);

    simpleColorShader->setUniformValue("pointSize", pointSize);


    vertexBuffer.bind();

    //TODO: these require a VBA to be bound; we need to create them I suppose.
    //enable the vertex attribute array in shader
    simpleColorShader->enableAttributeArray("vertex");
    //use attribute array from buffer, using non-normalized vertices
    gl->glVertexAttribPointer(simpleColorShader->attributeLocation("vertex"),
                              3, //3D vertices
                              GL_type, //type of data GL_FLAOT or GL_DOUBLE
                              GL_FALSE, //not normalized vertices!
                              0, //no spaceing between vertices in data
                              0 //start at offset 0 in buffer
                             );

    //draw primitive
    gl->glDrawArrays(operation,0,count);
    //release buffer
    vertexBuffer.release();
    //release shader
    simpleColorShader->release();
}

//Buffer should be a list of 7D tuples.  3 values for x,y,z,
//and 4 values for color: r,g,b,a.
void glDrawArraysColors(QOpenGLFunctions *gl,
                        QMatrix4x4 mvp,
                        GLenum operation,
                        QOpenGLBuffer &vertexBuffer,
                        GLenum GL_type,
                        int count,
                        float pointSize)
{
    //bind shader
    assert(interpColorShader->bind());
    //set mvp matrix
    interpColorShader->setUniformValue("mvpMatrix", mvp);

    interpColorShader->setUniformValue("pointSize", pointSize);


    vertexBuffer.bind();

    //enable the vertex attribute array in shader
    interpColorShader->enableAttributeArray("vertex");
    interpColorShader->enableAttributeArray("color");

    //use attribute array from buffer, using non-normalized vertices
    gl->glVertexAttribPointer(interpColorShader->attributeLocation("vertex"),
                              3, //3D vertices
                              GL_type, //type of data GL_FLAOT or GL_DOUBLE
                              GL_FALSE, //not normalized vertices!
                              7*sizeof(float), //vertex+color
                              0 //start at offset 0 in buffer
                             );

    gl->glVertexAttribPointer(interpColorShader->attributeLocation("color"),
                              4, //4D color
                              GL_type, //type of data GL_FLAOT or GL_DOUBLE
                              GL_FALSE, //not normalized vertices!
                              7*sizeof(float), //vertex+color
                              ((float *)0) + 3 //start at 3rd float in buffer
                             );

    //draw primitive
    gl->glDrawArrays(operation,0,count);
    //release buffer
    vertexBuffer.release();
    //release shader
    interpColorShader->release();
}

//Buffer should consist of 5D values.  3D for x,y,z, and 2D for
//texture x,y coordinate.
void glDrawArraysTexture(QOpenGLFunctions *gl,
                         QMatrix4x4 mvp,
                         GLenum operation,
                         QOpenGLBuffer &vertexBuffer,
                         GLenum GL_type,
                         int count)
{
    //bind shader
    assert(texShader->bind());
    //set mvp matrix
    texShader->setUniformValue("mvpMatrix", mvp);
    texShader->setUniformValue("texture", 0);
    texShader->setUniformValue("useColor", false);


    vertexBuffer.bind();

    //enable the vertex attribute array in shader
    texShader->enableAttributeArray("vertex");
    texShader->enableAttributeArray("texcoord_src");

    //use attribute array from buffer, using non-normalized vertices
    gl->glVertexAttribPointer(interpColorShader->attributeLocation("vertex"),
                              3, //3D vertices
                              GL_type, //type of data GL_FLAOT or GL_DOUBLE
                              GL_FALSE, //not normalized vertices!
                              5*sizeof(float), //vertex+color
                              0 //start at offset 0 in buffer
                             );

    gl->glVertexAttribPointer(interpColorShader->attributeLocation("texcoord_src"),
                              2, //2D coordinate
                              GL_type, //type of data GL_FLAOT or GL_DOUBLE
                              GL_FALSE, //not normalized vertices!
                              5*sizeof(float), //vertex+color
                              ((float *)0) + 3 //start at 3rd float in buffer
                             );

    //draw primitive
    gl->glDrawArrays(operation,0,count);
    //release buffer
    vertexBuffer.release();
    //release shader
    texShader->release();
}

GLHelperOneColor::GLHelperOneColor() {
}

void GLHelperOneColor::draw(QOpenGLFunctions *gl, QMatrix4x4 mvp, QColor color, GLenum operation, float point_size) {
    QOpenGLBuffer vertexBuffer;

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(this, size()*sizeof(QVector3D));
    vertexBuffer.release();


    glDrawArraysColor(gl, mvp,operation,
                      color, vertexBuffer, GL_FLOAT,
                      size(),point_size);

}

GLHelperColors::GLHelperColors() {
}

void GLHelperColors::draw(QOpenGLFunctions *gl, QMatrix4x4 mvp, GLenum operation, float point_size) {
    QOpenGLBuffer vertexBuffer;

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(this, size()*sizeof(ColorVertex));
    vertexBuffer.release();


    glDrawArraysColors(gl, mvp,operation,
                       vertexBuffer, GL_FLOAT,
                       size(),point_size);

}
