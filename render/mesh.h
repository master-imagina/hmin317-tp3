#ifndef MESH_H
#define MESH_H

#include <string>

#include "render/geometry/geometry.h"

struct aiMesh;
struct aiNode;
struct aiScene;


struct Mesh
{
    Mesh();
    explicit Mesh(const std::string &path);

    void setPath(const std::string &path);

    std::vector<Geometry> &geometries();

private:
    Geometry processMesh(aiMesh *mesh, const aiScene *scene);
    void processNode(aiNode *node, const aiScene *scene);

private:
    std::string m_path;

    std::vector<Geometry> m_geometries;
};

#endif // MESH_H
