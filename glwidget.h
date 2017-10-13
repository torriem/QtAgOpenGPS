#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <functional>

class QOpenGLContext;

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void setPaintGLCallback(std::function<void ()> callback);
    void setInitializeGLCallback(std::function<void ()> callback);
signals:
    void afterRender(); //signal emits after paintGL completes

protected:
    void initializeGL() override;
    void paintGL() override;
    virtual void resizeGL(int width, int height) override;
private:
    //paintGL calls this (in the main form)
    std::function<void ()> paintGL_callback;
    std::function<void ()> initializeGL_callback;
};

#endif // GLWIDGET_H
