#include "shapes.h"

void GeometryCube(Geometry * component) {
    Mesh mesh;
    // Vertex for face 0
    mesh.addVertex(QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.0f, 0.0f)); // v0
    mesh.addVertex(QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(1.0f, 0.0f)); // v1
    mesh.addVertex(QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)); // v2
    mesh.addVertex(QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(1.0f, 1.0f)); // v3

    // Vertex for face 1
    mesh.addVertex(QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.0f, 0.0f)); // v4
    mesh.addVertex(QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.0f)); // v5
    mesh.addVertex(QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)); // v6
    mesh.addVertex(QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(1.0f, 1.0f)); // v7

    // Vertex for face 2
    mesh.addVertex(QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.0f, 0.0f)); // v8
    mesh.addVertex(QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.0f)); // v9
    mesh.addVertex(QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.0f, 1.0f)); // v10
    mesh.addVertex(QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(1.0f, 1.0f)); // v11

    // Vertex for face 3
    mesh.addVertex(QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.0f, 0.0f)); // v12
    mesh.addVertex(QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(1.0f, 0.0f)); // v13
    mesh.addVertex(QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.0f, 1.0f)); // v14
    mesh.addVertex(QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(1.0f, 1.0f)); // v15

    // Vertex for face 4
    mesh.addVertex(QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.0f, 0.0f)); // v16
    mesh.addVertex(QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.0f)); // v17
    mesh.addVertex(QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.0f, 1.0f)); // v18
    mesh.addVertex(QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(1.0f, 1.0f)); // v19

    // Vertex for face 5
    mesh.addVertex(QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 0.0f)); // v20
    mesh.addVertex(QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(1.0f, 0.0f)); // v21
    mesh.addVertex(QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.0f, 1.0f)); // v22
    mesh.addVertex(QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(1.0f, 1.0f)); // v23

    // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
    mesh.addTriangle(0, 1, 2);
    mesh.addTriangle(1, 3, 2);
    // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
    mesh.addTriangle(4, 5, 6);
    mesh.addTriangle(5, 7, 6);
    // Face 2 - triangle strip ( v8,  v9, v10, v11)
    mesh.addTriangle(8, 9, 10);
    mesh.addTriangle(9, 11, 10);
    // Face 3 - triangle strip (v12, v13, v14, v15)
    mesh.addTriangle(12, 13, 14);
    mesh.addTriangle(13, 15, 14);
    // Face 4 - triangle strip (v16, v17, v18, v19)
    mesh.addTriangle(16, 17, 18);
    mesh.addTriangle(17, 19, 18);
    // Face 5 - triangle strip (v20, v21, v22, v23)
    mesh.addTriangle(20, 21, 22);
    mesh.addTriangle(21, 23, 22);

    component->setMesh(mesh);
}

void GeometryPlane(Geometry * component) {
    Mesh mesh;
    mesh.addVertex(QVector3D(-1.0f, 0.0f, -1.0f), QVector2D(0.0f, 0.0f));
    mesh.addVertex(QVector3D( 1.0f, 0.0f, -1.0f), QVector2D(1.0f, 0.0f));
    mesh.addVertex(QVector3D(-1.0f, 0.0f,  1.0f), QVector2D(0.0f, 1.0f));
    mesh.addVertex(QVector3D( 1.0f, 0.0f,  1.0f), QVector2D(1.0f, 1.0f));
    mesh.addTriangle(0, 2, 1);
    mesh.addTriangle(1, 2, 3);
    component->setMesh(mesh);
}

void GeometryUIPlane(Geometry * component) {
    Mesh mesh;
    mesh.addVertex(QVector3D(-1.0f, -1.0f, 0.0f), QVector2D(1.0f, 0.0f));
    mesh.addVertex(QVector3D( 1.0f, -1.0f, 0.0f), QVector2D(0.0f, 0.0f));
    mesh.addVertex(QVector3D(-1.0f, 1.0f,  0.0f), QVector2D(1.0f, 1.0f));
    mesh.addVertex(QVector3D( 1.0f, 1.0f,  0.0f), QVector2D(0.0f, 1.0f));
    mesh.addTriangle(0, 2, 1);
    mesh.addTriangle(1, 2, 3);
    component->setMesh(mesh);
}

void GeometryTerrain(Geometry * component, const QImage& heightmap) {
    int width = heightmap.width(),
        height = heightmap.height();
    Mesh mesh;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            mesh.addVertex(QVector3D(-1.0f + ((float)i / (float)(height - 1)) * 2,
                                     (float)(heightmap.pixel(i, j) & 0xff) / 1000.0,
                                     -1.0f + ((float)j / (float)(width - 1)) * 2),
                           QVector2D((float)i / (float)(height - 1), (float)(width - j - 1) / (float)(width - 1)));
        }
    }
    mesh.getVertex(0);
    for (int i = 0; i < (height - 1); i++) {
        for (int j = 0; j < (width - 1); j++) {
            mesh.addTriangle(i * width + j, i * width + j + 1, (i + 1) * width + j);
            mesh.addTriangle((i + 1) * width + j, i * width + j + 1, (i + 1) * width + j + 1);
        }
    }
    component->setMesh(mesh);
}
