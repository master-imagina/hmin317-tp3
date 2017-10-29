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
#include <iostream>

MainWidget::MainWidget(int fps, Seasons s, QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    height(0),
    sand(0),
    rock(0),
    angularSpeed(0),
    camera(),
    orbit(false),
    fps(fps),
    particleEngine(0),
    lightPos(4.0f, 10.0f, -4.0f)
{
    setMouseTracking(true);
    seasonTimer = new QTimer();
    seasonM = new SeasonManager(s);
    seasonTimer->connect(seasonTimer, SIGNAL(timeout()), seasonM, SLOT(changeSeason()));
    seasonTimer->start(15000);
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete height;
    delete sand;
    delete rock;
    delete snow_rock;
    delete snow_sand;
    delete geometries;
    delete particleEngine;
    doneCurrent();
}

//! [0]
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{

    if(e->button() == Qt::RightButton) {
       /* // Mouse release position - mouse press position
        QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

        // Rotation axis is perpendicular to the mouse position difference
        // vector
        QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

        // Accelerate angular speed relative to the length of the mouse sweep
        qreal acc = diff.length() / 100.0;

        // Calculate new rotation axis as weighted sum
        rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();
        // Increase angular speed
        angularSpeed += acc;*/
        orbit = !orbit;
    }
}
//! [0]

void MainWidget::wheelEvent(QWheelEvent *event) {
}

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{
    /*// Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
        // Request an update
        update();
    }*/
    update();
}
//! [1]

void MainWidget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_Z:
            camera.processKeyPress(Camera_Movement::Z);
            break;
        case Qt::Key_S:
            camera.processKeyPress(Camera_Movement::S);
            break;
        case Qt::Key_Q:
            camera.processKeyPress(Camera_Movement::Q);
            break;
        case Qt::Key_D:
            camera.processKeyPress(Camera_Movement::D);
            break;
        case Qt::Key_C:
            camera.processKeyPress(Camera_Movement::C);
            break;
        case Qt::Key_W:
            camera.processKeyPress(Camera_Movement::W);
            break;
    }
}

void MainWidget::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton) {
        float xoffset = event->x() - mousePressPosition.x();
        float yoffset = mousePressPosition.y() - event->y(); // reversed since y-coordinates range from bottom to top
        mousePressPosition = QVector2D(event->localPos());
        camera.processMouseMovement(xoffset, yoffset);
    }
}

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initTextures();
    initShaders();

//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    // Enable back face culling
    //glEnable(GL_CULL_FACE);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//! [2]
    geometries = new GeometryEngine;
    particleEngine = new ParticleEngine;

    // Use QBasicTimer because its faster than QTimer
    timer.start(1000/fps, this);
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

        // Compile vertex shader
        if (!particlesProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/prvshader.glsl"))
            close();

        // Compile fragment shader
        if (!particlesProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/prfshader.glsl"))
            close();

        // Link shader pipeline
        if (!particlesProgram.link())
            close();
}
//! [3]

//! [4]
void MainWidget::initTextures()
{
    // Load heightmap and cube.png image
    height = new QOpenGLTexture(QImage(":/heightmap-1.png").mirrored());
    sand= new QOpenGLTexture(QImage(":/sand.jpg").mirrored());
    rock= new QOpenGLTexture(QImage(":/rock.jpg").mirrored());
    snow_rock= new QOpenGLTexture(QImage(":/snow_rock.jpg").mirrored());
    snow_sand= new QOpenGLTexture(QImage(":/snow_sand.jpg").mirrored());

    // Set nearest filtering mode for texture minification
    height->setMinificationFilter(QOpenGLTexture::Nearest);
    sand->setMinificationFilter(QOpenGLTexture::Nearest);
    rock->setMinificationFilter(QOpenGLTexture::Nearest);
    snow_rock->setMinificationFilter(QOpenGLTexture::Nearest);
    snow_sand->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    height->setMagnificationFilter(QOpenGLTexture::Linear);
    sand->setMagnificationFilter(QOpenGLTexture::Linear);
    rock->setMagnificationFilter(QOpenGLTexture::Linear);
    snow_rock->setMagnificationFilter(QOpenGLTexture::Linear);
    snow_sand->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    height->setWrapMode(QOpenGLTexture::Repeat);
    sand->setWrapMode(QOpenGLTexture::Repeat);
    rock->setWrapMode(QOpenGLTexture::Repeat);
    snow_rock->setWrapMode(QOpenGLTexture::Repeat);
    snow_sand->setWrapMode(QOpenGLTexture::Repeat);
}
//! [4]

//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 0.3, zFar = 100.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]

void MainWidget::paintGL()
{
    program.bind();
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    height->bind(0);
    sand->bind(1);
    rock->bind(2);
    snow_rock->bind(3);
    snow_sand->bind(4);

//! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;
    if(orbit) {
        camera.orbitAround(matrix, 1.0f , 0.0f);
    }/*else {
        camera.lookAt(matrix);
    }*/
    camera.lookAt(matrix);

    matrix.rotate(rotation);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);
//! [6]
    program.setUniformValue("lightColor", QVector3D(1.0f, 1.0f, 1.0f));
    program.setUniformValue("height_map", 0);
    // Use texture unit 1 which contains cube.png
    if(seasonM->getSeason() != Seasons::Winter) {
        program.setUniformValue("sand", 1);
    } else {
        program.setUniformValue("sand", 4);
    }
    if(seasonM->getSeason() != Seasons::Winter) {
        program.setUniformValue("rock", 2);
    } else {
        program.setUniformValue("rock", 3);
    }
    // Draw cube geometry
    geometries->drawPlaneGeometry(&program);
    // draw particles
    particlesProgram.bind();
    particlesProgram.setUniformValue("mvp_matrix", projection * matrix);
    particlesProgram.setUniformValue("lightColor", QVector3D(1.0f, 1.0f, 1.0f));
    if(seasonM->getSeason() == Seasons::Winter) {
        particleEngine->generateParticles();
        particleEngine->updateParticles();
        particleEngine->drawParticles(&particlesProgram);
    }

}
