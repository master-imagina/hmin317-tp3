#include "assetmanager.h"

#include <iostream>


AssetManager *AssetManager::m_self = nullptr;


AssetManager::AssetManager() :
    m_bigFile(),
    m_images()
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

QImage *AssetManager::image(const std::string &path)
{
    const auto imageInstancied = m_images.find(path);

    if (imageInstancied == m_images.end()) {
        QImage img = QImage::fromData(m_bigFile.data(path));

        m_images.insert({path, img});
    }

    return &m_images[path];
}
