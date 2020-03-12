#include "glutils.h"
#include <QThread>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include "ccamera.h"
#include <assert.h>
#include <math.h>
#include "aogsettings.h"

//module-level symbols
QOpenGLShaderProgram *simpleColorShader = 0;
QOpenGLShaderProgram *texShader = 0;
QOpenGLShaderProgram *interpColorShader = 0;

QVector<QOpenGLTexture *> texture;

static int GlyphsPerLine = 16;
static int GlyphLineCount = 16;
static int GlyphWidth = 16;
static int GlyphHeight = 32;
static int CharXSpacing = 14;

int textureWidth;
int textureHeight;


bool isFontOn = true;

bool texturesLoaded = false;

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

void initializeTextures() {
    QOpenGLTexture *t;

    //  Background
    t = new QOpenGLTexture(QImage(":/images/textures/Landscape.png"));
    texture.append(t); //position 0
    t = new QOpenGLTexture(QImage(":/images/textures/floor.png"));
    t->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    texture.append(t); //position 1
    t = new QOpenGLTexture(QImage(":/images/textures/Font.png"));
    textureWidth = t->width();
    textureHeight= t->height();
    texture.append(t); //position 2
    t = new QOpenGLTexture(QImage(":/images/textures/Turn.png"));
    texture.append(t); //position 3
    t = new QOpenGLTexture(QImage(":/images/textures/TurnCancel.png"));
    texture.append(t); //position 4
    t = new QOpenGLTexture(QImage(":/images/textures/TurnManual.png"));
    texture.append(t); //position 5
    t = new QOpenGLTexture(QImage(":/images/textures/Compass.png"));
    texture.append(t); //position 6
    t = new QOpenGLTexture(QImage(":/images/textures/speedo.png"));
    texture.append(t); //position 7
    t = new QOpenGLTexture(QImage(":/images/textures/SpeedoNedle.png"));
    texture.append(t); //position 8
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

void destroyTextures() {

    foreach(const QOpenGLTexture *t, texture) {
        delete t;
    }
    texture.clear();
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
                         int count,
                         bool useColor = false,
                         QColor color = QColor::fromRgbF(1,1,1))
{
    gl->glEnable(GL_TEXTURE_2D);
    //bind shader
    assert(texShader->bind());
    //set mvp matrix
    texShader->setUniformValue("color", color);
    texShader->setUniformValue("texture", 0);
    texShader->setUniformValue("mvpMatrix", mvp);
    texShader->setUniformValue("useColor", useColor);


    vertexBuffer.bind();

    //enable the vertex attribute array in shader
    texShader->enableAttributeArray("vertex");
    texShader->enableAttributeArray("texcoord_src");

    //use attribute array from buffer, using non-normalized vertices
    gl->glVertexAttribPointer(texShader->attributeLocation("vertex"),
                              3, //3D vertices
                              GL_type, //type of data GL_FLAOT or GL_DOUBLE
                              GL_FALSE, //not normalized vertices!
                              5*sizeof(float), //vertex+color
                              0 //start at offset 0 in buffer
                             );

    gl->glVertexAttribPointer(texShader->attributeLocation("texcoord_src"),
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
    gl->glDisable(GL_TEXTURE_2D);
}

void drawText(QOpenGLFunctions *gl, QMatrix4x4 mvp, double x, double y, QString text, double size, bool colorize, QColor color)
{
    //GL.Color3(0.95f, 0.95f, 0.40f);

    GLHelperTexture gldraw;
    VertexTexcoord vt;

    double u_step = (double)GlyphWidth / (double)textureWidth;
    double v_step = (double)GlyphHeight / (double)textureHeight;

    for (int n = 0; n < text.length(); n++)
    {
        gldraw.clear();
        char idx = text.at(n).toLatin1();
        double u = (double)(idx % GlyphsPerLine) * u_step;
        double v = (double)(idx / GlyphsPerLine) * v_step;

        vt.texcoord = QVector2D(u, v);
        vt.vertex = QVector3D(x, y, 0);
        gldraw.append(vt);

        vt.texcoord = QVector2D(u + u_step, v);
        vt.vertex = QVector3D(x + GlyphWidth * size, y, 0);
        gldraw.append(vt);

        vt.texcoord = QVector2D(u, v + v_step);
        vt.vertex = QVector3D(x, y + GlyphHeight * size, 0);
        gldraw.append(vt);

        vt.texcoord = QVector2D(u + u_step, v + v_step);
        vt.vertex = QVector3D(x + GlyphWidth * size, y + GlyphHeight * size, 0);
        gldraw.append(vt);

        x += CharXSpacing * size;
        gldraw.draw(gl, mvp, Textures::FONT, GL_TRIANGLE_STRIP, colorize, color);
}    }



void drawText3D(const CCamera &camera, QOpenGLFunctions *gl,
                QMatrix4x4 mvp, double x1, double y1, QString text,
                double size, bool colorize, QColor color)
{
    USE_SETTINGS;

    GLHelperTexture gldraw;
    VertexTexcoord vt;

    double x = 0, y = 0;

    mvp.translate(x1, y1, 0);

    if (SETTINGS_DISPLAY_CAMPITCH < -45)
    {
        mvp.rotate(90, 1, 0, 0);
        if (camera.camFollowing) mvp.rotate(-camera.camHeading, 0, 1, 0);
        size = -camera.camSetDistance;
        size = pow(size, 0.8);
        size /= 800;
    }

    else
    {
        if (camera.camFollowing) mvp.rotate(-camera.camHeading, 0, 0, 1);
        size = -camera.camSetDistance;
        size = pow(size, 0.85);
        size /= 1000;
    }

    double u_step = (double)GlyphWidth / (double)textureWidth;
    double v_step = (double)GlyphHeight / (double)textureHeight;


    for (int n = 0; n < text.length(); n++)
    {
        char idx = text.at(n).toLatin1();
        double u = (double)(idx % GlyphsPerLine) * u_step;
        double v = (double)(idx / GlyphsPerLine) * v_step;
        gldraw.clear();

        vt.texcoord = QVector2D(u, v + v_step);
        vt.vertex = QVector3D(x, y, 0);
        gldraw.append(vt);

        vt.texcoord = QVector2D(u + u_step, v + v_step);
        vt.vertex = QVector3D(x + GlyphWidth * size, y, 0);
        gldraw.append(vt);

        vt.texcoord = QVector2D(u, v);
        vt.vertex = QVector3D(x, y + GlyphHeight * size, 0);
        gldraw.append(vt);

        vt.texcoord = QVector2D(u + u_step, v);
        vt.vertex = QVector3D(x + GlyphWidth * size, y + GlyphHeight * size, 0);
        gldraw.append(vt);
        gldraw.draw(gl, mvp, Textures::FONT, GL_TRIANGLE_STRIP, colorize, color);

        x += CharXSpacing * size;
    }

}

void drawTextVehicle(const CCamera &camera, QOpenGLFunctions *gl, QMatrix4x4 mvp,
                     double x, double y, QString text, double size, bool colorize, QColor color)
{
    USE_SETTINGS;
    GLHelperTexture gldraw;
    VertexTexcoord vt;

    size *= -camera.camSetDistance;
    size = pow(size, 0.8)/800;

    //2d
    if (SETTINGS_DISPLAY_CAMPITCH > -58)
    {
        if (!camera.camFollowing)
        {
            mvp.rotate(camera.camHeading, 0, 0, 1);
            y *= 1.2;
        }
        else
        {
            y *= 1.2;
            mvp.translate(x, y, 0);
            x = y = 0;
        }
    }
    //3d
    else
    {
        if (!camera.camFollowing)
        {
            mvp.rotate(90, 1, 0, 0);
            mvp.rotate(camera.camHeading, 0, 1, 0);
            y *= 0.3;
        }
        else
        {
            mvp.rotate(-SETTINGS_DISPLAY_CAMPITCH, 1, 0, 0);
            y *= 0.3;
        }
    }

    double u_step = (double)GlyphWidth / (double)textureWidth;
    double v_step = (double)GlyphHeight / (double)textureHeight;


    for (int n = 0; n < text.length(); n++)
    {
        char idx = text.at(n).toLatin1();
        double u = (double)(idx % GlyphsPerLine) * u_step;
        double v = (double)(idx / GlyphsPerLine) * v_step;

        vt.texcoord = QVector2D(u, v + v_step);
        vt.vertex = QVector3D(x, y, 0);
        gldraw.append(vt);

        vt.texcoord = QVector2D(u + u_step, v + v_step);
        vt.vertex = QVector3D(x + GlyphWidth * size, y, 0);
        gldraw.append(vt);

        vt.texcoord = QVector2D(u, v);
        vt.vertex = QVector3D(x, y + GlyphHeight * size, 0);
        gldraw.append(vt);

        vt.texcoord = QVector2D(u + u_step, v);
        vt.vertex = QVector3D(x + GlyphWidth * size, y + GlyphHeight * size, 0);
        gldraw.append(vt);

        x += CharXSpacing * size;
    }
    gldraw.draw(gl, mvp, Textures::FONT, GL_TRIANGLE_STRIP, colorize, color);
}

GLHelperOneColor::GLHelperOneColor() {
}

void GLHelperOneColor::draw(QOpenGLFunctions *gl, QMatrix4x4 mvp, QColor color, GLenum operation, float point_size) {
    QOpenGLBuffer vertexBuffer;

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(data(), size()*sizeof(QVector3D));
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
    vertexBuffer.allocate(data(), size()*sizeof(ColorVertex));
    vertexBuffer.release();


    glDrawArraysColors(gl, mvp,operation,
                       vertexBuffer, GL_FLOAT,
                       size(),point_size);

}

GLHelperTexture::GLHelperTexture() {

}

void GLHelperTexture::draw(QOpenGLFunctions *gl, QMatrix4x4 mvp, Textures textureno, GLenum operation, bool colorize, QColor color)
{
    QOpenGLBuffer vertexBuffer;
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(data(),size() * sizeof(VertexTexcoord));
    vertexBuffer.release();

    texture[textureno]->bind();
    glDrawArraysTexture(gl, mvp, operation, vertexBuffer, GL_FLOAT,size(),
                        colorize, color);
    texture[textureno]->release();
}
