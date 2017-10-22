#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "component.h"
#include "../geometry/mesh.h"

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

class Geometry : public Component, protected QOpenGLFunctions {

public:

    // Elements of vertex buffer
    struct VertexData {
        QVector3D position;
        QVector3D normal;
        QVector2D texCoord;
    };

protected:

    // Vertex buffer
    QOpenGLBuffer vertexBuffer;

    // Triangle index buffer
    QOpenGLBuffer indexBuffer;

public:

    // Basic constructor
    Geometry(GameObject * parent);

    // Get the geometry mesh
    Mesh getMesh();

    // Set the geometry mesh
    void setMesh(const Mesh& mesh);

    // Component override
    virtual void destroy() override;

    // Component override
    virtual int component() const override;

    // Component override
    virtual void clone(GameObject * c) override;

    // Indicate if the componant is an instance of Geometry
    static bool isInstance(Component * c);

    // Draw the mesh when rendering
    virtual void draw(QOpenGLShaderProgram *program);

};

#endif // GEOMETRY_H
