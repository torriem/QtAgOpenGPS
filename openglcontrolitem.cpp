#include "openglcontrolitem.h"
#include "openglcontrol.h"
#include <QQuickWindow>

OpenGLControlItem::OpenGLControlItem()
    :openglControl(0)
{
    connect(this, &QQuickItem::windowChanged, this, &OpenGLControlItem::handleWindowChanged);
}

void OpenGLControlItem::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &OpenGLControlItem::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &OpenGLControlItem::cleanup, Qt::DirectConnection);
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
    }
}

void OpenGLControlItem::cleanup()
{
    if (openglControl) {
        delete openglControl;
        openglControl=0;
    }
}

void OpenGLControlItem::sync()
{
    if (!openglControl) {
        openglControl = new OpenGLControl();
        connect(window(), &QQuickWindow::beforeRendering, openglControl, &OpenGLControl::paint, Qt::DirectConnection);
        emit controlSet(openglControl);
    }
    openglControl->setViewportSize(window()->size() * window()->devicePixelRatio());
    openglControl->setWindow(window());
}

OpenGLControl *OpenGLControlItem::renderer()
{
    return openglControl;
}


