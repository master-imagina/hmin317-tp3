#include  "geometry.h"
#include "gameobject.h"

Geometry::Geometry(GameObject * parent) :
    Component(parent),
    indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    vertexBuffer.create();
    indexBuffer.create();
}

Mesh Geometry::getMesh() {
    Mesh mesh;
    vertexBuffer.bind();
    unsigned int vertexSize = vertexBuffer.size();
    unsigned int vertexCount = vertexSize / sizeof(Geometry::VertexData);
    Geometry::VertexData * v_buffer = new Geometry::VertexData[vertexCount];
    vertexBuffer.read(0, (void *)v_buffer, vertexSize);
    for (unsigned int i = 0; i < vertexCount; i++)
        mesh.addVertex(v_buffer[i].position, v_buffer[i].texCoord);
    delete[] v_buffer;
    indexBuffer.bind();
    unsigned int indexSize = indexBuffer.size();
    unsigned int indexCount = indexSize / (3 * sizeof(GLuint));
    GLuint * i_buffer = new GLuint[indexCount * 3];
    indexBuffer.read(0, i_buffer, indexSize);
    for (unsigned int i = 0; i < indexCount; i++)
        mesh.addTriangle(i_buffer[i * 3], i_buffer[i * 3 + 1], i_buffer[i * 3 + 2]);
    delete[] i_buffer;
    return mesh;
}

void Geometry::setMesh(const Mesh& mesh) {
    QVector3D * normals = new QVector3D[mesh.vertexCount()];
    for (unsigned int i = 0, sz = mesh.trianglesCount(); i < sz; i++) {
        std::array<unsigned int, 3> t = mesh.getTriangle(i);
        QVector3D tmp = QVector3D::crossProduct(mesh.getVertex(t[1]) - mesh.getVertex(t[0]), mesh.getVertex(t[2]) - mesh.getVertex(t[0]));
        normals[t[0]] += tmp;
        normals[t[1]] += tmp;
        normals[t[2]] += tmp;
    }
    Geometry::VertexData * v_buffer = new Geometry::VertexData[mesh.vertexCount()];
    for (unsigned int i = 0, sz = mesh.vertexCount(); i < sz; i++) {
        v_buffer[i].position = mesh.getVertex(i);
        v_buffer[i].normal = normals[i].normalized();
        v_buffer[i].texCoord = mesh.getUV(i);
    }
    vertexBuffer.bind();
    vertexBuffer.allocate((void *)v_buffer, mesh.vertexCount() * sizeof(Geometry::VertexData));
    delete[] v_buffer;
    GLuint * i_buffer = new GLuint[mesh.trianglesCount() * 3];
    for (unsigned int i = 0, sz = mesh.trianglesCount(); i < sz; i++) {
        i_buffer[i * 3] = mesh.getTriangle(i)[0];
        i_buffer[i * 3 + 1] = mesh.getTriangle(i)[1];
        i_buffer[i * 3 + 2] = mesh.getTriangle(i)[2];
    }
    indexBuffer.bind();
    indexBuffer.allocate((void *)i_buffer, mesh.trianglesCount() * 3 * sizeof(GLuint));
    delete[] i_buffer;
}

void Geometry::destroy() {
    vertexBuffer.destroy();
    indexBuffer.destroy();
    removeComponent();
    delete this;
}

int Geometry::component() const {
    return COMPONENT_GEOMETRY_ID;
}

void Geometry::clone(GameObject * c) {
    Geometry * g = c->addComponent<Geometry>();
    g->setMesh(getMesh());
}

bool Geometry::isInstance(Component *c) {
    return (c->component() == COMPONENT_GEOMETRY_ID);
}

void Geometry::draw(QOpenGLShaderProgram *program) {
    vertexBuffer.bind();
    indexBuffer.bind();

    quintptr offset = 0;
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(Geometry::VertexData));

    offset += sizeof(QVector3D);
    int normalLocation = program->attributeLocation("a_normal");
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer(normalLocation, GL_FLOAT, offset, 3, sizeof(Geometry::VertexData));

    offset += sizeof(QVector3D);
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(Geometry::VertexData));

    glDrawElements(GL_TRIANGLES, indexBuffer.size() / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}
