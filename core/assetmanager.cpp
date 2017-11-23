#include "assetmanager.h"

#include <iostream>


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
