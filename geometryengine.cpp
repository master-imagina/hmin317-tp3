#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>
#include <QCoreApplication>
#include <QImage>
#include <QRgb>
#include <cstdlib>
#include <iostream>
using namespace std;
struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

GeometryEngine::GeometryEngine()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    initPlaneGeometry();
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}

void GeometryEngine::initCubeGeometry()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.0f, 0.0f)},  // v0
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.0f)}, // v1
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 0.5f)},  // v2
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v3

        // Vertex data for face 1
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D( 0.0f, 0.5f)}, // v4
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.5f)}, // v5
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)},  // v6
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v7

        // Vertex data for face 2
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v8
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.5f)},  // v9
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}, // v10
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(1.0f, 1.0f)},  // v11

        // Vertex data for face 3
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v12
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(1.0f, 0.0f)},  // v13
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v14
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(1.0f, 0.5f)},  // v15

        // Vertex data for face 4
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.0f)}, // v16
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v17
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v18
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v19

        // Vertex data for face 5
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v20
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v21
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v22
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}  // v23
    };

    // Indices for drawing cube faces using triangle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
    GLushort indices[] = {
         0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
         4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
         8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
        20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
    };

//! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 24 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 34 * sizeof(GLushort));
//! [1]
}

void GeometryEngine::initPlaneGeometry()
{
    int faces = 16; //nombre de face sur une ligne
    int nb_faces = faces*faces; //nimbre de face total (ligne * colonne)
    VertexData vertices[nb_faces * 4]; // *4 car chaque face est composé de 4 sommets

    int x = 0;
    int y = 0;
    int i = 0;
    int hauteur_max = 2;
    int iterator = 0;

    //tant que l'on a pas fais toute les faces
    while(iterator < nb_faces)
    {
        if(x == 0 && y == 0)//si c'est la première face
        {
            vertices[i + 0] = {QVector3D(x, y, generateRand(hauteur_max)), QVector2D(0.0f, 0.0f)};              // 3---4
            vertices[i + 1] = {QVector3D(x + 1, y, generateRand(hauteur_max)), QVector2D(0.33f, 0.0f)};         // -   -
            vertices[i + 2] = {QVector3D(x, y + 1, generateRand(hauteur_max)), QVector2D(0.0f, 0.5f)};          // -   -
            vertices[i + 3] = {QVector3D(x + 1, y + 1, generateRand(hauteur_max)), QVector2D(0.33f, 0.5f)};     // 1---2
        }
        else if ( x != 0 && y == 0 )// sinon si on est sur la première ligne
        {
            vertices[i + 0] = {QVector3D(vertices[i - 3].position), QVector2D(0.0f, 0.0f)};
            vertices[i + 1] = {QVector3D(x + 1, y, generateRand(hauteur_max)), QVector2D(0.33f, 0.0f)};
            vertices[i + 2] = {QVector3D(vertices[i - 1].position), QVector2D(0.0f, 0.5f)};
            vertices[i + 3] = {QVector3D(x + 1, y + 1, generateRand(hauteur_max)), QVector2D(0.33f, 0.5f)};
        }
        else if ( x == 0 && y != 0)// sinon si on est sur la première colonne
        {
            vertices[i + 0] = {QVector3D(vertices[(y - 1) * faces * 4 + 2 + 4 * x].position), QVector2D(0.0f, 0.0f)};
            vertices[i + 1] = {QVector3D(vertices[(y - 1) * faces * 4 + 3 + 4 * x].position), QVector2D(0.33f, 0.0f)};
            vertices[i + 2] = {QVector3D(x, y + 1, generateRand(hauteur_max)), QVector2D(0.0f, 0.5f)};
            vertices[i + 3] = {QVector3D(x + 1, y + 1, generateRand(hauteur_max)), QVector2D(0.33f, 0.5f)};
        }
        else
        {
            vertices[i + 0] = {QVector3D(vertices[i - 3].position), QVector2D(0.0f, 0.0f)};
            vertices[i + 1] = {QVector3D(vertices[(y - 1) * faces * 4 + 3 + 4 * x].position), QVector2D(0.33f, 0.0f)};
            vertices[i + 2] = {QVector3D(vertices[i - 1].position), QVector2D(0.0f, 0.5f)};
            vertices[i + 3] = {QVector3D(x + 1, y + 1, generateRand(hauteur_max)), QVector2D(0.33f, 0.5f)};
        }

        x++;

        //si on est arrivé au bout de la ligne alors
        //on revient au début et on passe à la ligne suivante
        if(x == faces)
        {
            x = 0;
            y++;
        }

        i += 4;

        //on l'incrémente car on vient de faire une face
        iterator++;
    }

    int taille_tableau_indice = nb_faces * 4 + nb_faces * 2 - 2; //4 * 256 + 256 * 2 - 2;
    GLushort indices[taille_tableau_indice] = {0};

    int q = 0;
    for (int j = 0; j < taille_tableau_indice; j += 6)
    {
        indices[j + 0] = q++;
        indices[j + 1] = q++;
        indices[j + 2] = q++;
        indices[j + 3] = q;

        if(j < (taille_tableau_indice - 6))
        {
            indices[j + 4] = q++;
            indices[j + 5] = q;
        }

    }

//! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, nb_faces * 4 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, taille_tableau_indice * sizeof(GLushort));
//! [1]
}

void GeometryEngine::initHeightmapGeometry()
{
    QImage image;
    image.load(":/heightmap-3.png");
    int Width = image.width();
    int Height = image.height();

    TailleImage = Width * Height;

    float Heightmap[Width][Height];

    //crée le tableau des nuances de gris de la HeightMap
    for(int i = 0; i < Height; i++)
    {
        for(int j = 0; j < Width; j++)
        {
            Heightmap[i][j] = qGray(image.pixel(i, j)) / 30;
        }
    }

    VertexData * vertices = (VertexData *)malloc(Width*Height*6*sizeof(VertexData));
    GLuint * indices = (GLuint *)malloc(Width*Height*6*sizeof(GLuint));


    //Fonction pas optimale mais fonctionne
    for(float i = 0; i < Height - 1; i++)
    {
        for(float j = 0; j < Width - 1; j++)
        {
            for(int k = 0; k < 6; k++)
            {
                //boucle qui crée les deux triangles d'une face et les insères directement dans le tableau d'indice
                //
                //  3--    6--5
                //  |         |
                //  1--2    --4
                //
                switch(k)
                {
                    case 0:
                        vertices[(int)(((i*Width)+j)*6+k)] = {QVector3D(i*0.3f, j*0.3f, Heightmap[(int)i][(int)j]), QVector2D(i/Width, j/Height)};
                        break;
                    case 1:
                        vertices[(int)(((i*Width)+j)*6+k)] = {QVector3D((i+1)*0.3f, j*0.3f, Heightmap[(int)i+1][(int)j]), QVector2D((i+1)/Width, j/Height)};
                        break;
                    case 2:
                        vertices[(int)(((i*Width)+j)*6+k)] = {QVector3D(i*0.3f, (j+1)*0.3f, Heightmap[(int)i][(int)j+1]), QVector2D(i/Width, (j+1)/Height)};
                        break;
                    case 3:
                        vertices[(int)(((i*Width)+j)*6+k)] = {QVector3D((i+1)*0.3f, j*0.3f, Heightmap[(int)i+1][(int)j]), QVector2D((i+1)/Width, j/Height)};
                        break;
                    case 4:
                        vertices[(int)(((i*Width)+j)*6+k)] = {QVector3D((i+1)*0.3f, (j+1)*0.3f, Heightmap[(int)i+1][(int)j+1]), QVector2D((i+1)/Width, (j+1)/Height)};
                        break;
                    case 5:
                        vertices[(int)(((i*Width)+j)*6+k)] = {QVector3D(i*0.3f, (j+1)*0.3f, Heightmap[(int)i][(int)j+1]), QVector2D(i/Width, (j+1)/Height)};
                        break;
                }
                indices[(int)(((i*Width)+j)*6+k)] = (GLuint)(((i*Width)+j)*6+k);
            }
        }
    }

    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, TailleImage * 6 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, TailleImage * 6 * sizeof(GLuint));
}

void GeometryEngine::drawCubeGeometry(QOpenGLShaderProgram *program)
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

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);
}

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

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, 3000, GL_UNSIGNED_SHORT, 0);
}

void GeometryEngine::drawHeightmapGeometry(QOpenGLShaderProgram *program)
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

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, TailleImage * 6, GL_UNSIGNED_INT, 0);
}

float GeometryEngine::generateRand(int max)
{
    return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max));
}
