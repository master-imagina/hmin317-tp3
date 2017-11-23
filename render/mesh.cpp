#include "mesh.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "core/assetmanager.h"

#include "render/renderassets.h"


////////////////////// Mesh //////////////////////

Mesh::Mesh() :
    m_path(),
    m_geometries()
{}

Mesh::Mesh(const std::string &path) :
    m_path(),
    m_geometries()
{
    setPath(path);
}

std::string Mesh::path() const
{
    return m_path;
}

void Mesh::setPath(const std::string &path)
{
    if (m_path != path) {
        m_path = path;

        Assimp::Importer importer;
        const aiScene *scene = assimpSceneFromFile(path, importer);

        if (!scene) {
            return;
        }

        processNode(scene->mRootNode, scene);
    }
}

int Mesh::count() const
{
    return m_geometries.size();
}

Geometry &Mesh::geometry(int index)
{
    return m_geometries.at(index);
}

void Mesh::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

        m_geometries.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Geometry Mesh::processMesh(aiMesh *mesh, const aiScene *scene)
{
    Geometry ret;
    ret.vertices.resize(mesh->mNumVertices);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position = {mesh->mVertices[i].x,
                           mesh->mVertices[i].y,
                           mesh->mVertices[i].z};

        if (mesh->mNormals) {
            vertex.normal = {mesh->mNormals[i].x,
                             mesh->mNormals[i].y,
                             mesh->mNormals[i].z};
        }

        if (mesh->mTextureCoords[0]) {
            vertex.texCoords = {mesh->mTextureCoords[0][i].x,
                                mesh->mTextureCoords[0][i].y};
        }

        ret.vertices[i] = vertex;
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            ret.indices.push_back(face.mIndices[j]);
        }
    }

    ret.vertexLayout.addAttribute(defaultPositionAttrib());
    ret.vertexLayout.addAttribute(defaultNormalAttrib());
    ret.vertexLayout.addAttribute(defaultTexCoordsAttrib());

    ret.primitiveType = Geometry::Triangles;
    ret.primitiveCount = mesh->mNumFaces;

    ret.isIndexed = true;
    ret.isDirty = true;

    return ret;
}
