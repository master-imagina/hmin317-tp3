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

#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>
#include <QtWidgets/QFileDialog>
#include <iostream>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

//! [0]
GeometryEngine::GeometryEngine(QWidget* qw)
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    // Initializes cube geometry and transfers it to VBOs
    initPlaneGeometry();
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}
//! [0]


void GeometryEngine::initPlaneGeometry()
{
    // Create array of 16 x 16 vertices facing the camera  (z=cte)
    VertexData vertices[16*16];

#pragma omp parallel for
    for (int i=0;i<16;i++)
        for (int j=0;j<16;j++)
            {
                // Vertex data for face 0
                vertices[16*i+j] = { QVector3D(0.1*(i-8),0.1*(j-8), 2), QVector2D(0.33*i/16.0,0.5*j/16.0)};
                // add height field eg (i-8)*(j-8)/256.0
        }


    // Draw 15 bands each with 32 vertices, with repeated vertices at the end of each band
    tailleIndices = 15 * 36;
    GLushort indices[tailleIndices];

#pragma omp parallel for
    for (int i=0;i<15;i++)
        {
            indices[36*i] = 16*i;
            indices[36*i+1] = 16*i;

            for (int j=2;j<34;j+=2)
                {
                    indices[36*i+j] = 16*i +(j-2)/2;
                    indices[36*i+j+1] = 16*(i+1) + (j-2)/2;
                }

            indices[36*i+34] = 16*(i+1) + 15;
            indices[36*i+35] = 16*(i+1) + 15;
    }

//! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 16*16 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, tailleIndices * sizeof(GLushort));
//! [1]
}

//Fonction modifiée pour faire un terrain avec une height map
void GeometryEngine::initPlaneGeometry(QWidget *qw)
{
/*
    //Dans un premier temps, une boite de dialogue pour choisir la height map
    QString fichier = QFileDialog::getOpenFileName(qw, "Ouvrir Heightmap","../","*.png");

    if (fichier.isEmpty() or fichier.isNull())
    {
        std::cout << "Aucun fichier choisi" << std::endl;
        std::exit(0);
    }
*/
    QImage heightMap(QString("../heightmap-1.png"));

    // Create array of the size of the map
    VertexData vertices[heightMap.width() * heightMap.height()];

#pragma omp parallel for
    for (int i=0;i<heightMap.height();i++)
        for (int j=0;j<heightMap.width();j++)
        {
            // Vertex data for face 0
            vertices[heightMap.width()*i+j] = {QVector3D(0.1*(i-8),0.1*(j-8), qGray(heightMap.pixel(i, j))/50.0), QVector2D(0.33*i/16.0,0.5*j/16.0)};
        }

    tailleIndices = (heightMap.height()-1)*(heightMap.width() + heightMap.height());
    GLushort indices[tailleIndices];

#pragma omp parallel for
    for (int i=0;i<(heightMap.height()-1);i++)
    {
        indices[(heightMap.width() + heightMap.height())*i] = heightMap.width()*i;
        indices[(heightMap.width() + heightMap.height())*i+1] = heightMap.width()*i;

        for (int j=2;j<(heightMap.width() + heightMap.height())-2;j+=2)
        {
            indices[(heightMap.width() + heightMap.height())*i+j] = heightMap.width()*i +(j-2)/2;
            indices[(heightMap.width() + heightMap.height())*i+j+1] = heightMap.width()*(i+1) + (j-2)/2;
        }

        indices[(heightMap.width() + heightMap.height())*i+(heightMap.width() + heightMap.height())-2] = heightMap.width()*(i+1) + heightMap.width()-1;
        indices[(heightMap.width() + heightMap.height())*i+(heightMap.width() + heightMap.height())-1] = heightMap.width()*(i+1) + heightMap.width()-1;
    }

//! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, heightMap.width() * heightMap.height() * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, tailleIndices * sizeof(GLushort));
//! [1]
}

//! [2]
void GeometryEngine::drawPlaneGeometry(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw plane geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, tailleIndices, GL_UNSIGNED_SHORT, 0);
}
//! [2]
