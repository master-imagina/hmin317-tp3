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

#include <math.h>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

MainWidget::MainWidget(int fps, int S, QWidget *parent) :
     QOpenGLWidget(parent),
     geometries(0),
     texture(0),
     angularSpeed(0.5),
     rotationAxis(0.0, 0.0, 1),
     fps(fps),
     season(S)
 {
    this->grabKeyboard();
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

void MainWidget::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();

    /*if (key == Qt::Key_4) {e->accept(); imput[0] = true;}
    if (key == Qt::Key_6) {e->accept(); imput[1] = true;}
    if (key == Qt::Key_8) {e->accept(); imput[2] = true;}
    if (key == Qt::Key_2) {e->accept(); imput[3] = true;}

    if (key == Qt::Key_1) {e->accept(); imput[6] = true;}
    if (key == Qt::Key_3) {e->accept(); imput[7] = true;}
    if (key == Qt::Key_7) {e->accept(); imput[8] = true;}
    if (key == Qt::Key_9) {e->accept(); imput[9] = true;}

    if (key == Qt::Key_Plus) {e->accept(); imput[4] = true;}
    if (key == Qt::Key_Minus) {e->accept(); imput[5] = true;}

    if (key == Qt::Key_Left) if(imput[11] == false) {imput[11] = true;} else {imput[11] = false;}
    if (key == Qt::Key_Right) if(imput[12] == false) {imput[12] = true;} else {imput[12] = false;}*/

    if (key == Qt::Key_4) {e->accept(); projection.translate(-1,0,0); update();}
    if (key == Qt::Key_6) {e->accept(); projection.translate(1,0,0); update();}
    if (key == Qt::Key_8) {e->accept(); projection.translate(0,1,0); update();}
    if (key == Qt::Key_2) {e->accept(); projection.translate(0,-1,0); update();}

    if (key == Qt::Key_Plus) {e->accept(); angularSpeed +=1; update();}
    if (key == Qt::Key_Minus) {e->accept(); angularSpeed -=1; update();}

    update();
}

void MainWidget::keyReleaseEvent(QKeyEvent *e)
{
    /*int key = e->key();

    if (key == Qt::Key_4) {e->accept(); imput[0] = false;}
    if (key == Qt::Key_6) {e->accept(); imput[1] = false;}
    if (key == Qt::Key_8) {e->accept(); imput[2] = false;}
    if (key == Qt::Key_2) {e->accept(); imput[3] = false;}

    if (key == Qt::Key_1) {e->accept(); imput[6] = false;}
    if (key == Qt::Key_3) {e->accept(); imput[7] = false;}
    if (key == Qt::Key_7) {e->accept(); imput[8] = false;}
    if (key == Qt::Key_9) {e->accept(); imput[9] = false;}

    if (key == Qt::Key_Plus) {e->accept(); imput[4] = false;}
    if (key == Qt::Key_Minus) {e->accept(); imput[5] = false;}

    update();*/
}

//! [0]
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;
    QVector3D n = QVector3D(0.0,0.0,1.0).normalized();
    qreal acc = diff.length() / 100.0;
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();
    angularSpeed += acc;
}
//! [0]

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{
    /*if (imput[0]) scanX += 1;
    if (imput[1]) scanX -= 1;

    if (imput[2]) scanY -= 1;
    if (imput[3]) scanY += 1;

    if (imput[4]) scanZ += 1;
    if (imput[5]) scanZ -= 1;

    if (imput[6]) {scanX += 1; scanY += 1;}
    if (imput[7]) {scanX -= 1; scanY += 1;}
    if (imput[8]) {scanX += 1; scanY -= 1;}
    if (imput[9]) {scanX -= 1; scanY -= 1;}*/

    rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

    update();
}
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

    geometries = new GeometryEngine;

    // Use QBasicTimer because its faster than QTimer
    timer.start(1000/fps, this);
}

//! [3]
void MainWidget::initShaders()
{
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();
    if (!program.link())
        close();
    if (!program.bind())
        close();
}
//! [3]

//! [4]
void MainWidget::initTextures()
{
    // Load cube.png image
    texture = new QOpenGLTexture(QImage(":/heightmap-1.jpg"));
    scanX = 0;
    scanY = 0;
    scanZ = 0;

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
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    const qreal zNear = 3.0, zFar = 500.0, fov = 90.0;
    projection.setToIdentity();
    projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]

void MainWidget::newSeason()
{
   if (season == 0) season = 1;
   else if (season == 1) season = 2;
   else if (season == 2) season = 3;
   else if (season == 3) season = 0;
   cout << "season :" << season << endl;
   update();
}

void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture->bind();

//! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;

    matrix.translate(0, 0, -4.0);

    //QQuaternion framing = QQuaternion::fromAxisAndAngle(QVector3D(1,0,0),-45.0);
    matrix.rotate(rotation);
    matrix.rotate(-45, 1, 0, 0);

    // QVector3D eye = QVector3D(0.0,0.5,-5.0);
    // QVector3D center = QVector3D(0.0,0.0,2.0);
    // QVector3D up = QVector3D(-1,0,0);
    // matrix.lookAt(eye,center,up);


    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);

    if (season == 0) {
        program.setUniformValue("r", 0.2f);
        program.setUniformValue("g", 1.0f);
        program.setUniformValue("b", 0.2f);
    }

    else if (season == 1) {
        program.setUniformValue("r", 0.8f);
        program.setUniformValue("g", 0.8f);
        program.setUniformValue("b", 0.1f);
    }

    else if (season == 2) {
        program.setUniformValue("r", 1.0f);
        program.setUniformValue("g", 0.2f);
        program.setUniformValue("b", 0.2f);
    }

    else if (season == 3) {
        program.setUniformValue("r", 0.2f);
        program.setUniformValue("g", 0.2f);
        program.setUniformValue("b", 1.0f);
    }

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);
    // Draw cube geometry
    geometries->drawPlaneGeometry(&program);
}
