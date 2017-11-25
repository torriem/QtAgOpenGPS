#include "aogrenderer.h"
#include <QOpenGLContext>
#include <QQuickWindow>
#include <QOpenGLFramebufferObject>

#include <QOpenGLFunctions_2_1>

#include "formgps.h"

void AOGRenderer::render()
{
    /*
    QOpenGLContext *c = QOpenGLContext::currentContext();


    QOpenGLFunctions_2_1 *gl = c->versionFunctions<QOpenGLFunctions_2_1>();
    gl->initializeOpenGLFunctions(); //necessary here?

    gl->glClearColor(0.22f,0.2858f,0.16f , 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT);
//gl->glViewport(0, 0, 400, 400);
    gl->glMatrixMode(GL_PROJECTION);
    gl->glLoadIdentity();
    gl->glOrtho(-2.0, 2.0, -2.0, 2.0, -1.5, 1.5);
    //gl->glFrustum(-2.0, 2.0, -2.0, 2.0, 1.5, 20.0);

    gl->glMatrixMode(GL_MODELVIEW);

    //glClearColor(0.22f, 0.2858f, 0.16f, 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT);
    gl->glLoadIdentity();

    //gluLookAt(0.0, 0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    gl->glColor3f(1.0f, 1.0f, 1.0f);

    double size = 2 * 0.5;

#   define V(a,b,c) gl->glVertex3d( a size, b size, c size );
#   define N(a,b,c) gl->glNormal3d( a, b, c );

    gl->glLineWidth(5);
    // PWO: I dared to convert the code to use macros...
    gl->glBegin( GL_LINE_LOOP ); N( 1.0, 0.0, 0.0); V(+,-,+); V(+,-,-); V(+,+,-); V(+,+,+); gl->glEnd();
    gl->glBegin( GL_LINE_LOOP ); N( 0.0, 1.0, 0.0); V(+,+,+); V(+,+,-); V(-,+,-); V(-,+,+); gl->glEnd();
    gl->glBegin( GL_LINE_LOOP ); N( 0.0, 0.0, 1.0); V(+,+,+); V(-,+,+); V(-,-,+); V(+,-,+); gl->glEnd();
    gl->glBegin( GL_LINE_LOOP ); N(-1.0, 0.0, 0.0); V(-,-,+); V(-,+,+); V(-,+,-); V(-,-,-); gl->glEnd();
    gl->glBegin( GL_LINE_LOOP ); N( 0.0,-1.0, 0.0); V(-,-,+); V(-,-,-); V(+,-,-); V(+,-,+); gl->glEnd();
    gl->glBegin( GL_LINE_LOOP ); N( 0.0, 0.0,-1.0); V(-,-,-); V(-,+,-); V(+,+,-); V(+,-,-); gl->glEnd();
    gl->glLineWidth(1);

#   undef V
#   undef N

    gl->glFlush();
    */
/*
    //f->glClearColor( red, green, blue, 1.0f);
    f->glClearColor(0.22f,0.2858f,0.16f , 1.0f);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    f->glCullFace(GL_BACK);
    f->glClear(GL_COLOR_BUFFER_BIT);
    ourwindow->resetOpenGLState();
    std::cout << "." << std::flush;
    */
    /*
    if(paintCallback) {
        //std::cout << "Attempting to call paint callback." << std::endl;

        paintCallback(c);
    }
    */
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

