#include "mesh.h"

unsigned int Mesh::vertexCount() const {
    return (unsigned int)V.size();
}

unsigned int Mesh::trianglesCount() const {
    return (unsigned int)T.size();
}

unsigned int Mesh::addVertex(const QVector3D &v, const QVector2D &uv) {
    V.push_back(v);
    UV.push_back(uv);
    return (unsigned int)V.size() - 1;
}

unsigned int Mesh::addTriangle(unsigned int a, unsigned int b, unsigned int c) {
    T.push_back({ a, b, c });
    return (unsigned int)T.size() - 1;
}

QVector3D& Mesh::getVertex(unsigned int i) {
    return V[i];
}

const QVector3D& Mesh::getVertex(unsigned int i) const {
    return V[i];
}

QVector2D& Mesh::getUV(unsigned int i) {
    return UV[i];
}

const QVector2D& Mesh::getUV(unsigned int i) const {
    return UV[i];
}

std::array<unsigned int, 3>& Mesh::getTriangle(unsigned int i) {
    return T[i];
}

const std::array<unsigned int, 3>& Mesh::getTriangle(unsigned int i) const {
    return T[i];
}

Mesh& Mesh::operator=(const Mesh& m) {
    V = m.V;
    UV = m.UV;
    T = m.T;
    return *this;
}
