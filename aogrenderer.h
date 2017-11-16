#ifndef OPENGLCONTROL_H
#define OPENGLCONTROL_H

#include <QQuickWindow>
//#include <QOpenGLContext>
#include <QQuickFramebufferObject>

#include <functional>

class FormGPS;

class AOGRenderer : public QQuickFramebufferObject::Renderer
{
protected:
    virtual void synchronize(QQuickFramebufferObject *);

    FormGPS *mf;
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
    bool calledInit = false;

    //callback in main form to do actual rendering
    /*
    std::function<void ()> paintCallback;
    std::function<void ()> initCallback;
    std::function<void ()> cleanupCallback;
    */
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
