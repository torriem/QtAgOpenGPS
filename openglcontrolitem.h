#ifndef OPENGLCONTROLITEM_H
#define OPENGLCONTROLITEM_H

#include <QQuickItem>

class OpenGLControl;

class OpenGLControlItem : public QQuickItem
{
    Q_OBJECT
public:
    OpenGLControlItem();

    OpenGLControl *renderer();


signals:
    void controlSet(OpenGLControl *);

public slots:
    void sync();
    void cleanup();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    OpenGLControl *openglControl;
};

#endif // OPENGLCONTROLITEM_H
