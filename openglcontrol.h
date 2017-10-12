#ifndef OPENGLCONTROL_H
#define OPENGLCONTROL_H

#include <QQuickItem>
#include <QOpenGLContext>
#include <QObject>

#include <functional>

class OpenGLControl : public QObject
{
    Q_OBJECT
public:
    OpenGLControl();
    ~OpenGLControl();

    void setViewportSize(const QSize &size) {viewportSize = size; }
    void setWindow(QQuickWindow *window) { ourwindow = window; }

    //can't use the "set" prefix because it has special meaning
    //to a QObject-derived class.
    void registerPaintCallback(std::function<void (QOpenGLContext *)> callback);
    void registerInitCallback(std::function<void (QOpenGLContext *)> callback);

    QSize viewportSize;
signals:

public slots:
    void paint();

private:
    QQuickWindow *ourwindow;
    bool calledInit = false;

    //callback in main form to do actual rendering
    std::function<void (QOpenGLContext *)> paintCallback;
    std::function<void (QOpenGLContext *)> initCallback;
};

#endif // OPENGLCONTROL_H
