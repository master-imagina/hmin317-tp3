#include "mesh.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "extras/assetmanager.h"


Mesh::Mesh() :
    m_path()
{}

Mesh::Mesh(const std::string &path) :
    m_path(path)
{
    setPath(path);
}

void Mesh::setPath(const std::string &path)
{
    Assimp::Importer importer;

    const QByteArray data = AssetManager::self()->bigFile().data(path);

    const aiScene* scene =  importer.ReadFileFromMemory(data, data.size(),
                                                        aiProcess_Triangulate |
                                                        aiProcess_FlipUVs |
                                                        aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "[ERROR] Assimp: " << importer.GetErrorString() << std::endl;
        return;
    }

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

std::vector<Geometry> &Mesh::geometries()
{
    return m_geometries;
}

void Mesh::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

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
        vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

        if (mesh->mTextureCoords[0]) {
            vertex.texCoords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        }

        ret.vertices[i] = vertex;
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            ret.indices.push_back(face.mIndices[j]);
        }
    }

    // process materials
    //    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    //    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    //    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    //    // 2. specular maps
    //    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    //    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    //    // 3. normal maps
    //    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    //    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    //    // 4. height maps
    //    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    //    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    VertexAttrib standardVertexAttrib {"vertexPos", 3, VertexAttrib::Type::Float, false, Geometry::vertexSize};
    VertexAttrib standardNormalAttrib {"normal", 3, VertexAttrib::Type::Float, false, Geometry::vertexSize, offsetof(Vertex, normal)};
    VertexAttrib standardTexCoordsAttrib {"texCoords", 2, VertexAttrib::Type::Float, false, Geometry::vertexSize, offsetof(Vertex, texCoords)};

    ret.vertexLayout.addAttribute(standardVertexAttrib);
    ret.vertexLayout.addAttribute(standardNormalAttrib);
    ret.vertexLayout.addAttribute(standardTexCoordsAttrib);

    ret.primitiveType = Geometry::Triangles;
    ret.primitiveCount = mesh->mNumFaces;

    ret.isIndexed = true;
    ret.isDirty = true;

    return ret;
}
