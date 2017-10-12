#include "openglcontrol.h"
#include <QOpenGLContext>
#include <QQuickWindow>
#include <iostream> //for cout debugging

#include <QOpenGLFunctions_2_0>

void OpenGLControl::paint()
{
    QOpenGLContext *c = ourwindow->openglContext();

    if (initCallback && !calledInit) {
        //std::cout << "Attempting to call init callback." << std::endl;
        initCallback(c);
        calledInit=true;
    }

    QOpenGLFunctions_2_0 *gl = c->versionFunctions<QOpenGLFunctions_2_0>();
    gl->initializeOpenGLFunctions(); //necessary here?

    gl->glViewport(0, 0, 400, 400);
    gl->glMatrixMode(GL_PROJECTION);
    gl->glLoadIdentity();
    gl->glOrtho(-2.0, 2.0, -2.0, 2.0, -1.5, 1.5);
    //glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
    gl->glMatrixMode(GL_MODELVIEW);

    //glClearColor(0.22f, 0.2858f, 0.16f, 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT);
    gl->glLoadIdentity();

    //gluLookAt(0.0, 0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    gl->glColor3f(1.0f, 1.0f, 1.0f);

    double size = 2 * 0.5;

#   define V(a,b,c) gl->glVertex3d( a size, b size, c size );
#   define N(a,b,c) gl->glNormal3d( a, b, c );

    gl->glLineWidth(3);
    /* PWO: I dared to convert the code to use macros... */
    gl->glBegin( GL_LINE_LOOP ); N( 1.0, 0.0, 0.0); V(+,-,+); V(+,-,-); V(+,+,-); V(+,+,+); gl->glEnd();
    gl->glBegin( GL_LINE_LOOP ); N( 0.0, 1.0, 0.0); V(+,+,+); V(+,+,-); V(-,+,-); V(-,+,+); gl->glEnd();
    gl->glBegin( GL_LINE_LOOP ); N( 0.0, 0.0, 1.0); V(+,+,+); V(-,+,+); V(-,-,+); V(+,-,+); gl->glEnd();
    gl->glBegin( GL_LINE_LOOP ); N(-1.0, 0.0, 0.0); V(-,-,+); V(-,+,+); V(-,+,-); V(-,-,-); gl->glEnd();
    gl->glBegin( GL_LINE_LOOP ); N( 0.0,-1.0, 0.0); V(-,-,+); V(-,-,-); V(+,-,-); V(+,-,+); gl->glEnd();
    gl->glBegin( GL_LINE_LOOP ); N( 0.0, 0.0,-1.0); V(-,-,-); V(-,+,-); V(+,+,-); V(+,-,-); gl->glEnd();

#   undef V
#   undef N

    gl->glFlush();
/*
    //f->glClearColor( red, green, blue, 1.0f);
    f->glClearColor(0.22f,0.2858f,0.16f , 1.0f);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    f->glCullFace(GL_BACK);
    f->glClear(GL_COLOR_BUFFER_BIT);
    ourwindow->resetOpenGLState();
    std::cout << "." << std::flush;
    */
    /*
    if(paintCallback) {
        //std::cout << "Attempting to call paint callback." << std::endl;

        paintCallback(c);
    }
    */

    ourwindow->resetOpenGLState();
}

OpenGLControl::OpenGLControl()
{
}

OpenGLControl::~OpenGLControl()
{
}

void OpenGLControl::registerPaintCallback(std::function<void (QOpenGLContext *)> callback) {
    paintCallback = callback;
    //std::cout << "paint callback is registered." << std::endl;
}

void OpenGLControl::registerInitCallback(std::function<void (QOpenGLContext *)> callback) {
    initCallback = callback;
    //std::cout << "init callback is registered." << std::endl;
}


