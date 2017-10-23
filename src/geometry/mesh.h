#ifndef MESH_H
#define MESH_H

#include <vector>
#include <array>

#include <QVector2D>
#include <QVector3D>

class Mesh {

    // Vertex list
    std::vector<QVector3D> V;

    // UV coordinates list
    std::vector<QVector2D> UV;

    // Triangle indexs list
    std::vector<std::array<unsigned int, 3>> T;

public:

    // Get vertex count
    unsigned int vertexCount() const;

    // Get triangles count
    unsigned int trianglesCount() const;

    // Add a new vertex
    // Return the vertex ID
    unsigned int addVertex(const QVector3D& v, const QVector2D& uv);

    // Add a new triangle
    // Return the triangle ID
    unsigned int addTriangle(unsigned int a, unsigned int b, unsigned int c);

    // Get a vertex
    QVector3D& getVertex(unsigned int i);

    // Get a vertex
    const QVector3D& getVertex(unsigned int i) const;

    // Get a vertex UV coordinates
    QVector2D& getUV(unsigned int i);

    // Get a vertex UV coordinates
    const QVector2D& getUV(unsigned int i) const;

    // Get a triangle indexs
    std::array<unsigned int, 3>& getTriangle(unsigned int i);

    // Get a triangle indexs
    const std::array<unsigned int, 3>& getTriangle(unsigned int i) const;

    // Assign operator
    Mesh& operator=(const Mesh& m);

};

#endif // MESH_H
