// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
#include <QOpenGLFunctions_2_1>
#include <QOpenGLContext>
#include <QSurface>
#include "openglcontrol.h"



void gltest_init (){


}

void gltest_draw (){
    QOpenGLContext *glContext = QOpenGLContext::currentContext();

    QOpenGLFunctions_2_1 *gl = glContext->versionFunctions<QOpenGLFunctions_2_1>();
    int width = glContext->surface()->size().width();
    int height = glContext->surface()->size().height();


    //glViewport(0, 0, c->viewportSize.width(), c->viewportSize.height());
    //gl->glViewport(0, 0, 400, 400);
    gl->glMatrixMode(GL_PROJECTION);
    gl->glLoadIdentity();
    gl->glOrtho(-2.0, 2.0, -2.0, 2.0, -1.5, 1.5);
    //glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
    gl->glMatrixMode(GL_MODELVIEW);

    glClearColor(0.22f, 0.2858f, 0.16f, 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT);
    gl->glLoadIdentity();

    //gluLookAt(0.0, 0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    gl->glColor3f(1.0f, 1.0f, 1.0f);

    double size = 2 * 0.5;

#   define V(a,b,c) gl->glVertex3d( a size, b size, c size );
#   define N(a,b,c) gl->glNormal3d( a, b, c );

    gl->glLineWidth(3);
    /* PWO: I dared to convert the code to use macros... */
    gl->glBegin( GL_LINE_LOOP ); N( 1.0, 0.0, 0.0); V(+,-,+); V(+,-,-); V(+,+,-); V(+,+,+); glEnd();
    gl->glBegin( GL_LINE_LOOP ); N( 0.0, 1.0, 0.0); V(+,+,+); V(+,+,-); V(-,+,-); V(-,+,+); glEnd();
    gl->glBegin( GL_LINE_LOOP ); N( 0.0, 0.0, 1.0); V(+,+,+); V(-,+,+); V(-,-,+); V(+,-,+); glEnd();
    gl->glBegin( GL_LINE_LOOP ); N(-1.0, 0.0, 0.0); V(-,-,+); V(-,+,+); V(-,+,-); V(-,-,-); glEnd();
    gl->glBegin( GL_LINE_LOOP ); N( 0.0,-1.0, 0.0); V(-,-,+); V(-,-,-); V(+,-,-); V(+,-,+); glEnd();
    gl->glBegin( GL_LINE_LOOP ); N( 0.0, 0.0,-1.0); V(-,-,-); V(-,+,-); V(+,+,-); V(+,-,-); glEnd();

#   undef V
#   undef N

    gl->glFlush();
}
