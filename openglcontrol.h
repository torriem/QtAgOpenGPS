#ifndef OPENGLCONTROL_H
#define OPENGLCONTROL_H

#include <QQuickItem>
#include <QOpenGLFunctions_1_1>
#include <QOpenGLContext>
#include <QObject>

class OpenGLControl : public QObject, protected QOpenGLFunctions_1_1
{
    Q_OBJECT
public:
    OpenGLControl();
    ~OpenGLControl();

    void setViewportSize(const QSize &size) {viewportSize = size; }
    void setWindow(QQuickWindow *window) { ourwindow = window; }

    //callback here

    QSize viewportSize;
signals:

public slots:
    void paint();

private:
    QQuickWindow *ourwindow;
};

#endif // OPENGLCONTROL_H
