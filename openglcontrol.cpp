#include "openglcontrol.h"
#include <QOpenGLContext>
#include <QQuickWindow>
#include <iostream> //for cout debugging

void OpenGLControl::paint()
{
    QOpenGLContext *c = ourwindow->openglContext();

    if (initCallback && !calledInit) {
        //std::cout << "Attempting to call init callback." << std::endl;
        initCallback(c);
        calledInit=true;
    }

    /*
    QOpenGLFunctions_1_1 *f = c->versionFunctions<QOpenGLFunctions_1_1>();
    f->initializeOpenGLFunctions();
    //f->glClearColor( red, green, blue, 1.0f);
    f->glClearColor(0.22f,0.2858f,0.16f , 1.0f);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    f->glCullFace(GL_BACK);
    f->glClear(GL_COLOR_BUFFER_BIT);
    ourwindow->resetOpenGLState();
    std::cout << "." << std::flush;
    */
    if(paintCallback) {
        //std::cout << "Attempting to call paint callback." << std::endl;

        paintCallback(c);
    }

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


