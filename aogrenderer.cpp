#include "aogrenderer.h"
#include <QOpenGLContext>
#include <QQuickWindow>
#include <QOpenGLFramebufferObject>

#include "formgps.h"

void AOGRenderer::render()
{
    //update();
    if(mf) {
        if (!isInitialized) {
            //call the main form initialize gl function
            mf->openGLControl_Initialized();
            isInitialized = true;
        }
        mf->openGLControl_Draw();
    }
    //win->resetOpenGLState();
}

AOGRenderer::AOGRenderer():
    isInitialized(false),win(0),calledInit(false), samples(0)
{
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

    QVariant prop = fbo->property("mainform");
    if (prop.isValid()) {
        mf = (FormGPS *)prop.value<void *>();
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

