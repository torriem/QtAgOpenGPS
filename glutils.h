#ifndef GLUTILS_H
#define GLUTILS_H

#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QColor>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QVector3D>
#include <QVector4D>
#include <QVector2D>
#include "vec2.h"
#include "vec3.h"

class CCamera;

struct ColorVertex {
    QVector3D vertex;
    QVector4D color;
};

struct VertexTexcoord {
    QVector3D vertex;
    QVector2D texcoord;
};

enum Textures {
    SKY=0,
    FLOOR=1,
    FONT=2,
    TURN=3,
    TURNCANCEL=4,
    TURNMANUAL=5,
    COMPASS=6,
    SPEEDO=7,
    SPEEDONEDLE=8,
    HYDLIFT=9,
    SKYNIGHT=10,
    STEER_POINTER=11,
    STEER_DOT=12,
    TRACTOR=13,
    QUESTION_MARK=14,
    FRONT_WHEELS=15,
    TRACTOR_4WD_FRONT=16,
    TRACTOR_4WD_REAR=17,
    HARVESTER=18,
    LATERAL_MANUAL=19,
    BING_MAP=20, //unused
    NOGPS=21,
    ZOOM_IN=22,
    ZOOM_OUT=23,
    PAN=24,
    TOOLWHEELS=25
};

extern bool isFontOn;
extern QVector<QOpenGLTexture *> texture;


//thinking about putting GL buffer drawing routines here
//like Draw box, etc. Do I put the shaders as module globals here?

void initializeShaders();
void initializeTextures();

void destroyShaders();
void destroyTextures();

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
                         int count, bool useColor, QColor color);
//draw arrays

void drawText(QOpenGLFunctions *gl, QMatrix4x4 mvp, double x, double y, QString text, double size = 1.0, bool colorize = false, QColor color = QColor::fromRgbF(1,1,1));
void drawText3D(const CCamera &camera, QOpenGLFunctions *gl, QMatrix4x4 mvp, double x1, double y1, QString text, double size = 1.0, bool colorize = false, QColor color = QColor::fromRgbF(1,1,1));
void drawTextVehicle(const CCamera &camera, QOpenGLFunctions *gl, QMatrix4x4 mvp, double x, double y, QString text, double size = 1.0, bool colorize = false, QColor color = QColor::fromRgbF(1,1,1));

void DrawPolygon(QOpenGLFunctions *gl, QMatrix4x4 mvp, QVector<Vec2> &polygon, float size, QColor color);
void DrawPolygon(QOpenGLFunctions *gl, QMatrix4x4 mvp, QVector<Vec3> &polygon, float size, QColor color);

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

class GLHelperTexture: public QVector<VertexTexcoord>
{
public:
    GLHelperTexture();
    void draw(QOpenGLFunctions *gl, QMatrix4x4 mvp, Textures textureno, GLenum operation,
              bool colorize=false, QColor color = QColor::fromRgbF(1,1,1));
};

#endif // GLUTILS_H
