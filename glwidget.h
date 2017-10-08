#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_1_1>
#include <functional>

class QOpenGLContext;

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void setPaintGLCallback(std::function<void (QOpenGLContext *)> callback);
    void setInitializeGLCallback(std::function<void (QOpenGLContext *)> callback);

protected:
    void initializeGL() override;
    void paintGL() override;
    virtual void resizeGL(int width, int height) override;
private:
    //paintGL calls this (in the main form)
    std::function<void (QOpenGLContext *)> paintGL_callback;
    std::function<void (QOpenGLContext *)> initializeGL_callback;
};

#endif // GLWIDGET_H
