/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwidget.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include <math.h>
#include <QTimer>
#include <QPainter>
#include <QCursor>
#include <iostream>
MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    texture(0),
    angularSpeed(0),
    fps(1000),
    speedController(1.01)

{

}

MainWidget::MainWidget(QWidget *parent, int fps, int season, QTimer *seasonTimer) :
    QOpenGLWidget(parent),
    fps(fps),
    geometries(0),
    texture(0),
    angularSpeed(0),
    speedController(1.01),
    season(season),
    seasonTimer(seasonTimer)
{
    connect(seasonTimer,SIGNAL(timeout()),this,SLOT(switchSeason()));
    /*generateParticleTimer = new QTimer();
    connect(generateParticleTimer,SIGNAL(timeout()),this,SLOT(generateParticle()));
    generateParticleTimer->start(10);*/
    emitter = new ParticleEmitter(QVector3D(0,100,0),297);
    c = cursor();


}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    delete geometries;
    doneCurrent();
}

//! [0]
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
    QMouseEvent *cursorevent = dynamic_cast<QMouseEvent *>(e);
    if(cursorevent)
        input.mousePressed(cursorevent->button());
    if(input.getMouseLeftButton()){
        c.setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
        c.setShape(Qt::BlankCursor);
        setCursor(c);
    }

}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{

    QMouseEvent *cursorEvent = dynamic_cast<QMouseEvent *>(e);
    if(cursorEvent)
        input.mouseReleased(cursorEvent->button());
    c.setShape(Qt::ArrowCursor);
    setCursor(c);

}
//! [0]
void MainWidget::mouseMoveEvent(QMouseEvent *e)
{

}


void MainWidget::keyPressEvent(QKeyEvent *event)
{
    QKeyEvent *c = dynamic_cast<QKeyEvent *>(event);
    if(c)
        input.keyPressed(c->key());
    if (c && c->key() ==  Qt::Key_Up){
        angularSpeed *=1.01;
    }
    if (c && c->key() ==  Qt::Key_Down){
        angularSpeed *=0.99;
    }
}

void MainWidget::keyReleaseEvent(QKeyEvent *event)
{

    QKeyEvent *c = dynamic_cast<QKeyEvent *>(event);
    if(c)
        input.keyReleased(c->key());

    if (c && c->key() ==  Qt::Key_Up){
        speedController *= 1.01;
    }
    if (c && c->key() ==  Qt::Key_Down){
        speedController *=0.99;
    }

}


//! [0]

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{
    update();
}


//! [1]
//! [1]

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

    //! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    //! [2]
    camera.move(0,0,0,0,0,0,0);

    geometries = new GeometryEngine;
    // Use QBasicTimer because its faster than QTimer
    int msec = 1000/fps;
    timer.start(msec, this);
    this->elapsedTimer.start();

    qreal aspect = qreal(this->size().width()) / qreal(this->size().height());
    const qreal zNear = 1.0, zFar = 1000.0, fov = 45.0;

    projection.perspective(fov, aspect, zNear, zFar);
    camera.setProjectionMatrix(projection);



}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();


}
//! [3]

//! [4]
void MainWidget::initTextures()
{
    // Load cube.png image
    texture = new QOpenGLTexture(QImage(":/heightmap-3.png").mirrored());

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
}
//! [4]

//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 1.0, far plane to 10.0, field of view 45 degrees
    const qreal zNear = 1.0, zFar = 1000.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);

    camera.setProjectionMatrix(projection);
}
//! [5]

void MainWidget::switchSeason(){
    if(season == 4){
        season = 1;
    }else{
        season++;
    }
}

void MainWidget::paintGL()
{
    // Clear color and depth buffer
    QPoint pos = mapFromGlobal(QCursor::pos());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    texture->bind();
    if(input.getMouseLeftButton())
        camera.move(pos.x()-width()/2,pos.y()-height()/2,input.getKeyZ(),input.getKeyS(),input.getKeyQ(),input.getKeyD(),input.getKeyShift()-input.getKeySpace());
    else
        camera.move(0,0,input.getKeyZ(),input.getKeyS(),input.getKeyQ(),input.getKeyD(),input.getKeyShift()-input.getKeySpace());

    dx=0,dy=0;

    // Draw cube geometry

    QMatrix4x4 mvp = camera.getProjectionMatrix()*camera.getViewMatrix();

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", mvp);

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("season", season);
    if(season == 3 || season == 2){
        emitter->update();
        emitter->updateParticle();
    }
    geometries->drawPlaneGeometry(&program);


    glPointSize(15);
    glBegin(GL_POINTS);

    glVertex3f(0, 50,0);

    glEnd();


    if(input.getMouseLeftButton()){
        c.setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
        setCursor(c);
    }
}
