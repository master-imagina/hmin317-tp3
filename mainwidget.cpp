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

MainWidget::MainWidget(QWidget *parent, int _fps, int _saison) :
    QOpenGLWidget(parent),
    geometries(0),
    texture(0),
    angularSpeed(1), fps(_fps), saison(_saison)
{
    updateSeason();
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    delete geometries;

    if (!posParticles->isNull())
        delete[] posParticles;

    delete posParticles;

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
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis along the z axis
    //QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
    QVector3D n = QVector3D(0.0,0.0,diff.x()).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}
//! [0]

//! [1]
/*
void MainWidget::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Request an update
        update();
    }
}
*/
void MainWidget::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    //angularSpeed *= .99;

    // Stop rotation when speed goes below threshold
    /*if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {*/
        // Update rotation
        //rotation = QQuaternion::fromAxisAndAngle(QVector3D(0.0,0.0,1.0), vitesse) * rotation;

        // Request an update
        update();
    //}
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

    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

    //int posLum[4] = {0,5,0,1};
    //glLightiv(GL_LIGHT0, GL_POSITION, posLum); //Positionnement de la lumière (restera fixe)
    //glLightfv(GL_LIGHT0, GL_POSITION, light); //Couleur de la lumière

    geometries = new GeometryEngine(parentWidget());

    // Use QBasicTimer because its faster than QTimer
    timer.start(fps, this);
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
    texture = new QOpenGLTexture(QImage(":/floor.jpg").mirrored());

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
    const qreal zNear = 1.0, zFar = 100.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]

void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture->bind();

//! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;

    matrix.translate(posx, 0, posy);

    QQuaternion framing = QQuaternion::fromAxisAndAngle(QVector3D(1,0,0),-45.0);
    matrix.rotate(framing);

    matrix.translate(0.0, -1.8, 0.0);

    // QVector3D eye = QVector3D(0.0,0.5,-5.0);
    // QVector3D center = QVector3D(0.0,0.0,2.0);
    // QVector3D up = QVector3D(-1,0,0);
    // matrix.lookAt(eye,center,up);

    matrix.rotate(rotation);

    //glLightfv(GL_LIGHT0, GL_POSITION, light); //Changement de la lumière si besoin


    // Use color for the ground
    program.setUniformValue("a_color", groundColor);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    // Draw cube geometry
    geometries->drawPlaneGeometry(&program);

    //Particles

    if (particles > 0)
    {
        glBegin(GL_POINTS);
        if (saison == AUTOMNE)
            glColor3f(0,0,1);
        else
            glColor3f(1,1,1);
#pragma omp parallel for
        for (int i = 0; i < particles; ++i)
        {
            posParticles[i].setY(posParticles[i].y() - (GRAVITY) * mass);
            if (posParticles[i].y() < 0)
            {
                posParticles[i] = QVector3D(rand() % 10 - 5, rand() % 10 + 10, rand() % 10 - 5); //On repositionne directement la particule
            }
            glVertex3f(posParticles[i].x(), posParticles[i].y(), posParticles[i].z());
        }
        glEnd();

    }
}

void MainWidget::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Escape)
        std::exit(0);

    //Reception des inputs
    float _x = (float)(e->key() == Qt::Key_Right || e->key() == Qt::Key_D) - (float)(e->key() == Qt::Key_Left || e->key() == Qt::Key_Q);
    float _y = (float)(e->key() == Qt::Key_Up    || e->key() == Qt::Key_Z) - (float)(e->key() == Qt::Key_Down || e->key() == Qt::Key_S);

    vitesse += e->key() == Qt::Key_Plus ? 1 : 0;
    vitesse -= e->key() == Qt::Key_Minus ? 1 : 0;

    posx += _x/10.f;
    posy += _y/10.f;

    update(); //Il faut mettre a jour la scene !

}

void MainWidget::nextSeason() {
    ++saison;
    saison %= 4;
    updateSeason();

}

void MainWidget::updateSeason() {
    switch (saison)
    {
        case PRINTEMPS:
            setWindowTitle("Printemps");
            particles = 0;
            groundColor = QVector4D(0.8,1.0,0.8,1.0);

            if (posParticles)
                delete[] posParticles;

            break;

        case ETE:
            setWindowTitle("Été");
            particles = 0;
            groundColor = QVector4D(1.0,1.0,0.5,1.0);
            break;

        case AUTOMNE:
            setWindowTitle("Automne");
            particles = 150;
            mass = 0.3;
            groundColor = QVector4D(1.0,0.6,0.0,1.0);

            posParticles = new QVector3D[particles];

#pragma omp parallel for
            for (int i = 0; i < particles; ++i)
            {
                posParticles[i] = QVector3D(rand() % 10 - 5, rand() % 10 + 10, rand() % 10 - 5);
            }

            break;

        case HIVER:
            setWindowTitle("Hiver");
            particles = 500;
            mass = .01;
            groundColor = QVector4D(0.9,0.9,1.0,1.0);

            posParticles = new QVector3D[particles];

            #pragma omp parallel for
            for (int i = 0; i < particles; ++i)
            {
                posParticles[i] = QVector3D(rand() % 10 - 5, rand() % 10 + 10, rand() % 10 - 5);
            }

            break;
    }
}