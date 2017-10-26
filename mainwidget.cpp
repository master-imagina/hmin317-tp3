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
#include "ModelParticle.hpp"

#include <QMouseEvent>

#include <math.h>
#include <iostream>
#include <sstream>

MainWidget::MainWidget(QWidget *parent, int fps, int start_saison, float worldTime):
    QOpenGLWidget(parent),
    tex_ground(0), tex_p_snow(0), tex_p_flower(0), tex_p_rain(0), tex_p_leaf(0),
    angularSpeed(0.5),
    m_fps(fps),
    m_worldTime(worldTime),
    m_mpWinter(nullptr, 30.f, 0.05f, 0, QVector3D(0.3f,0.3f,0.0f)),
    m_mpSpring(nullptr, 30.f, 0.0f),
    m_mpSummer(nullptr, 1.f, 1.f, -2.0f, QVector3D(1.5f,1.5f,0.0f), true),
    m_mpAutomn(nullptr, 30.f, 0.1f),
    m_ep(nullptr),
    m_actualSaison(start_saison)
{
    rotationAxis.setZ(1.f);

    QString title("Terrain à ");
    title += QString::number(m_fps);
    title += " fps";
    setWindowTitle(title);
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete tex_ground;
    delete geometries;
    doneCurrent();
}

void MainWidget::timerEvent(QTimerEvent *)
{
    // Update rotation
    float delta = static_cast<float>(m_timer.restart());
    double newAngle = angularSpeed * (60.0 / (1000.0 / delta)) * (double)m_worldTime;
    rotation = QQuaternion::fromAxisAndAngle(rotationAxis, (float)newAngle) * rotation;

    // __________ PARTICULES _______
    m_ep->update(delta);

    // Request an update
    update();
}

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    geometries = new GeometryEngine;
    geometries->initPlaneGeometry();

    // ____ PARTICULES ____
    m_ep = new ParticleEmitter(1000, m_mpWinter, QVector3D(), 2.5f, 4.f);
    setSaison(m_actualSaison);

    // Use QBasicTimer because its faster than QTimer
    int milliSleep = 1000 / m_fps;
    timer.start(milliSleep, this);
}

void MainWidget::initShaders()
{
    // Compile basic vertex shader
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

void MainWidget::initTextures()
{
    // Load ground.png image
    tex_ground = new QOpenGLTexture(QImage(":/ground.jpg").mirrored());

    // Set nearest filtering mode for texture minification
    tex_ground->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    tex_ground->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    tex_ground->setWrapMode(QOpenGLTexture::Repeat);

    // _______ PARTICLES ______
    tex_p_snow = new QOpenGLTexture(QImage(":/part_snow.png"));
    tex_p_snow->setMinificationFilter(QOpenGLTexture::Nearest);
    tex_p_snow->setMagnificationFilter(QOpenGLTexture::Linear);
    m_mpWinter.setTexture(tex_p_snow);

    tex_p_flower = new QOpenGLTexture(QImage(":/part_flower.png"));
    tex_p_flower->setMinificationFilter(QOpenGLTexture::Nearest);
    tex_p_flower->setMagnificationFilter(QOpenGLTexture::Linear);
    m_mpSpring.setTexture(tex_p_flower);

    tex_p_rain = new QOpenGLTexture(QImage(":/part_rain.png"));
    tex_p_rain->setMinificationFilter(QOpenGLTexture::Nearest);
    tex_p_rain->setMagnificationFilter(QOpenGLTexture::Linear);
    m_mpSummer.setTexture(tex_p_rain);

    tex_p_leaf = new QOpenGLTexture(QImage(":/part_leaf.png"));
    tex_p_leaf->setMinificationFilter(QOpenGLTexture::Nearest);
    tex_p_leaf->setMagnificationFilter(QOpenGLTexture::Linear);
    m_mpAutomn.setTexture(tex_p_leaf);
}

void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 1.0, far plane to 10.0, field of view 45 degrees
    const qreal zNear = 0.5, zFar = 100.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}

void MainWidget::paintGL()
{
    // ________ Dessin du sol __________
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    tex_ground->bind();

    // Calculate model view transformation
    QMatrix4x4 matrix;

    matrix.translate(0.0, -1.1, -3.0);

    QQuaternion framing = QQuaternion::fromAxisAndAngle(QVector3D(1,0,0),-85.0);
    matrix.rotate(framing);

    //matrix.translate(0.0, 0.0, 0.0);
    matrix.scale(5.0f);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    // Use color for the ground
    program.setUniformValue("a_color", groundColor);

    // Draw cube geometry
    geometries->drawPlaneGeometry(&program);

    // ________ Dessin des particules __________
    m_ep->setPostion(QVector3D(0.0, -1.0, -4.0));
    m_ep->draw(projection, &program);
}

void MainWidget::setSaison(int saison)
{
    // ParticleEmitter(1000, m_mpWinter, QVector3D(), 2.5f, 4.f);
    switch(saison)
    {
        case WINTER:
            m_ep->setNbParticles(1000);
            m_ep->setModel(m_mpWinter);
            m_ep->setHeight(2.5f);
            m_ep->setRadius(4.f);
            groundColor = QVector4D(1.3f,1.3f,1.3f,1.f);
            setWindowTitle("Hiver");
            break;

        case SPRING:
            m_ep->setNbParticles(50);
            m_ep->setModel(m_mpSpring);
            m_ep->setHeight(0);
            m_ep->setRadius(4.f);
            m_ep->resetAllParticles();
            groundColor = QVector4D(0.7f,1.1f,0.7f,1.f);
            setWindowTitle("Printemps");
            break;

        case SUMMER:
            m_ep->setNbParticles(100);
            m_ep->setModel(m_mpSummer);
            m_ep->setHeight(2.5f);
            m_ep->setRadius(3.f);
            m_ep->resetAllParticles();
            groundColor = QVector4D(0.8f,0.8f,0.5f,1.f);
            setWindowTitle("Ete (en bretagne)");
            break;

        case AUTOMN:
            m_ep->setNbParticles(500);
            m_ep->setModel(m_mpAutomn);
            m_ep->setHeight(2.5f);
            m_ep->setRadius(3.f);
            m_ep->resetAllParticles();
            groundColor = QVector4D(1.0f,0.7f,0.5f,1.f);
            setWindowTitle("Automne");
            break;
    }
}

void MainWidget::nextSaison()
{
    m_actualSaison++;
    m_actualSaison %= 4;

    setSaison(m_actualSaison);
}
