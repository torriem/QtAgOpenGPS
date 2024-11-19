// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
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

    //notify main loop we're done.
    emit afterRender();
}

void GLWidget::resizeGL(int _width, int _height) {
    //This should really only be used to resize FBOs and such.
    //identity matrix stuff should go in paintGL.
}
