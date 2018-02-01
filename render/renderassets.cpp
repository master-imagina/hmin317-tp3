#include "renderassets.h"

#include <iostream>

#include <QFile>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "core/assetmanager.h"


namespace {

void readShaderFromFile(const std::string &filePath, std::string &sourceCode)
{
    if (filePath.empty()) {
        return;
    }

    AssetManager *assetManager = AssetManager::self();

    if (assetManager->bigFile().entryCount() == 0) {
        const QString assetPath = assetManager->pathFor(QString::fromStdString(filePath));

        QFile file(assetPath);

        if (!file.open(QFile::ReadOnly)) {
            std::cerr << "[ERROR] shaderProgramFromFile() : "
                      << "can't open " << filePath
                      << std::endl;
        }
        else {
            sourceCode = file.readAll().toStdString();
        }
    }
    else {
        sourceCode = assetManager->bigFile().data(filePath).toStdString();
    }
}

} // anon namespace


ShaderProgram shaderProgramFromFile(const std::string &vertFilePath,
                                    const std::string &fragFilePath)
{
    ShaderProgram ret;

    readShaderFromFile(vertFilePath, ret.vertexShaderSource);
    readShaderFromFile(fragFilePath, ret.fragmentShaderSource);

    return ret;
}

ShaderProgram shaderProgramFromFile(const std::string &vertFilePath,
                                    const std::string &geomFilePath,
                                    const std::string &fragFilePath)
{
    ShaderProgram ret;

    readShaderFromFile(vertFilePath, ret.vertexShaderSource);
    readShaderFromFile(geomFilePath, ret.geometryShaderSource);
    readShaderFromFile(fragFilePath, ret.fragmentShaderSource);

    return ret;
}

QImage imageFromFile(const std::string &path)
{
    QImage ret;

    AssetManager *assetManager = AssetManager::self();

    if (assetManager->bigFile().entryCount() == 0) {
        const QString assetPath = assetManager->pathFor(QString::fromStdString(path));

        const bool loaded = ret.load(assetPath);

        if (!loaded) {
            std::cerr << "[ERROR] imageFromFile() : "
                      << "can't open " << path
                      << std::endl;
        }
    }
    else {
        ret = QImage::fromData(assetManager->bigFile().data(path));
    }

    return ret;
}

const aiScene *assimpSceneFromFile(const std::string &path,
                                   Assimp::Importer &importer)
{
    AssetManager *assetManager = AssetManager::self();

    const unsigned int pFlags =
            aiProcess_Triangulate |
            aiProcess_FlipUVs |
            aiProcess_CalcTangentSpace;

    if (assetManager->bigFile().entryCount() == 0) {
        const QString assetPath = assetManager->pathFor(QString::fromStdString(path));

        const aiScene *ret = importer.ReadFile(assetPath.toStdString(), pFlags);

        if (!ret || ret->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ret->mRootNode) {
            std::cerr << "[ERROR] Assimp: " << importer.GetErrorString() << std::endl;
            return nullptr;
        }

        return ret;
    }
    else {
        const QByteArray data = AssetManager::self()->bigFile().data(path);

        const aiScene *ret = importer.ReadFileFromMemory(data, data.size(), pFlags);

        if (!ret || ret->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ret->mRootNode) {
            std::cerr << "[ERROR] Assimp: " << importer.GetErrorString() << std::endl;
            return nullptr;
        }

        return ret;
    }
}
