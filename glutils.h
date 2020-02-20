#ifndef GLUTILS_H
#define GLUTILS_H

#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QColor>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QVector4D>
#include <QVector2D>

struct ColorVertex {
    QVector3D vertex;
    QVector4D color;
};

struct VertexTexcoord {
    QVector3D vertex;
    QVector2D texcoord;
};

//thinking about putting GL buffer drawing routines here
//like Draw box, etc. Do I put the shaders as module globals here?

void initializeShaders();

void destroyShaders();

//Simple wrapper to draw primitives using lists of Vec3 or QVector3Ds
//with a single color.
void glDrawArraysColor(QOpenGLFunctions *gl, QMatrix4x4 mvp,
                       GLenum operation, QColor color,
                       QOpenGLBuffer &vertexBuffer, GLenum glType,
                       int count,
                       float pointSize=1.0f);
//Simple wrapper to draw primitives using lists of vec3s or QVector3Ds
//with a color per vertex. Buffer format is 7 values per vertice:
//x,y,z,r,g,b,a
void glDrawArraysColors(QOpenGLFunctions *gl, QMatrix4x4 mvp,
                       GLenum operation,
                       QOpenGLBuffer &vertexBuffer, GLenum glType,
                       int count,
                       float pointSize=1.0f);

//Buffer format is 5 values per vertice:
//x,y,z,texX,texY
void glDrawArraysTexture(QOpenGLFunctions *gl, QMatrix4x4 mvp,
                         GLenum operation,
                         QOpenGLBuffer &vertexBuffer, GLenum glType,
                         int count);
//draw arrays


class GLHelperOneColor: public QVector<QVector3D>
{
public:
    GLHelperOneColor();
    void draw(QOpenGLFunctions *gl, QMatrix4x4 mvp, QColor color, GLenum operation, float point_size);
};

class GLHelperColors: public QVector<ColorVertex>
{
public:
    GLHelperColors();
    void draw(QOpenGLFunctions *gl, QMatrix4x4 mvp, GLenum operation, float point_size);
};

#endif // GLUTILS_H
