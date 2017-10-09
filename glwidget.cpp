#include <QtOpenGL>
#include <GL/glut.h>
#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    :QOpenGLWidget(parent)
{

}

GLWidget::~GLWidget()
{
}

void GLWidget::setPaintGLCallback(std::function<void ()> callback) {
    paintGL_callback = callback;
}

void GLWidget::setInitializeGLCallback(std::function<void ()> callback) {
    initializeGL_callback = callback;

}

void GLWidget::initializeGL() {

    //call the callback so the main form can load textures
    if(initializeGL_callback) initializeGL_callback();

}

void GLWidget::paintGL() {
    //call back to the main form since this needs access to main form
    //variables.
    if(paintGL_callback) paintGL_callback();
}

void GLWidget::resizeGL(int _width, int _height) {
    //This should really only be used to resize FBOs and such.
    //identity matrix stuff should go in paintGL.
}
