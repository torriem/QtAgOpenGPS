// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// OpenGL renderer. Displayed in QML
#ifndef OPENGLCONTROL_H
#define OPENGLCONTROL_H

#include <QQuickWindow>
//#include <QOpenGLContext>
#include <QQuickFramebufferObject>

#include <functional>
Q_DECLARE_METATYPE(std::function<void (void)>)

class AOGRenderer : public QQuickFramebufferObject::Renderer
{
protected:
    virtual void synchronize(QQuickFramebufferObject *);

    bool isInitialized;

public:
    AOGRenderer();
    ~AOGRenderer();

    void render();

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);

    /*
    void registerPaintCallback(std::function<void ()> callback);
    void registerInitCallback(std::function<void ()> callback);
    void registerCleanupCallback(std::function<void ()> callback);
    */

private:
    QQuickWindow *win;
    bool calledInit;
    //FormGPS *mf;
    int samples;

    //callback in main form to do actual rendering
    void *callback_object;
    std::function<void (void)> paintCallback;
    std::function<void (void)> initCallback;
    std::function<void (void)> cleanupCallback;
};

class AOGRendererInSG : public QQuickFramebufferObject
{
    Q_OBJECT
public:
    AOGRenderer *theRenderer;

    AOGRendererInSG();

    AOGRenderer *createRenderer() const;
};

#endif // OPENGLCONTROL_H
