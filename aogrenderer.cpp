#include "aogrenderer.h"
#include <QOpenGLContext>
#include <QQuickWindow>
#include <QOpenGLFramebufferObject>

void AOGRenderer::render()
{
    //update();
    if(callback_object && initCallback) {
        if (!isInitialized) {
            //call the main form initialize gl function
            initCallback( callback_object );
            isInitialized = true;
        }
        paintCallback( callback_object );
    }
    //win->resetOpenGLState();
}

AOGRenderer::AOGRenderer():
    isInitialized(false),win(0),calledInit(false), samples(0)
{
    callback_object = NULL;
    initCallback = NULL;
    paintCallback = NULL;
    cleanupCallback = NULL;

    qDebug() << "AOGRenderer constructor here.";
}

AOGRenderer::~AOGRenderer()
{
    //call gl cleanup method in main form.
}

void AOGRenderer::synchronize(QQuickFramebufferObject *fbo)
{
    //get window
    win = fbo->window();

    QVariant prop = fbo->property("callbackObject");
    if (prop.isValid()) {
        callback_object = (void *)prop.value<void *>();
    }

    prop = fbo->property("initCallback");
    if (prop.isValid()) {
        initCallback = (void (*)(void *))prop.value<void *>();
    }

    prop = fbo->property("paintCallback");
    if (prop.isValid()) {
        paintCallback = (void (*)(void *))prop.value<void *>();
    }

    prop = fbo->property("cleanupCallback");
    if (prop.isValid()) {
        cleanupCallback = (void (*)(void *))prop.value<void *>();
    }

    prop = fbo->property("samples");
    if (prop.isValid()) {
        samples = prop.toInt();
    }
}

AOGRendererInSG::AOGRendererInSG()
{
    theRenderer = NULL;
}

QOpenGLFramebufferObject *AOGRenderer::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    if(samples) {
        format.setSamples(samples);
    }
    return new QOpenGLFramebufferObject(size,format);
}

AOGRenderer *AOGRendererInSG::createRenderer() const
{
    return new AOGRenderer();
}

