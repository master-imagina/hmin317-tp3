#include "assetmanager.h"

#include <iostream>

#include "render/material/shaderutils.h"


AssetManager *AssetManager::m_self = nullptr;


AssetManager::AssetManager() :
    m_bigFile()
{}

AssetManager *AssetManager::self()
{
    if (!m_self) {
        m_self = new AssetManager;
    }

    return m_self;
}

const BigFile &AssetManager::bigFile()
{
    return m_bigFile;
}

void AssetManager::loadBigFile(const std::string &path)
{
    m_bigFile = importBigFile(path);
}

QStringList AssetManager::supportedImageFormats()
{
    return {"png", "jpg", "jpeg", "bmp"};
}

QImage AssetManager::image(const std::string &path)
{
    return QImage::fromData(m_bigFile.data(path));
}

uptr<ShaderProgram> AssetManager::shader(const std::string &vertShaderPath,
                                         const std::string &geomShaderPath,
                                         const std::string &fragShaderPath)
{
    return shaderProgramFromSource(m_bigFile.data(vertShaderPath).toStdString(),
                                   m_bigFile.data(geomShaderPath).toStdString(),
                                   m_bigFile.data(fragShaderPath).toStdString());
}
