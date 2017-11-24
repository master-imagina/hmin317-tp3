#include "assetmanager.h"

#include <QDebug>
#include <QFileInfo>
#include <QUrl>


AssetManager *AssetManager::m_self = nullptr;


AssetManager::AssetManager() :
    m_searchPath(),
    m_bigFile()
{}

AssetManager *AssetManager::self()
{
    if (!m_self) {
        m_self = new AssetManager;
    }

    return m_self;
}

QString AssetManager::searchPath() const
{
    return m_searchPath;
}

void AssetManager::setSearchPath(const QString &path)
{
    if (m_searchPath != path) {
        const QFileInfo folderInfo(path);

        if (folderInfo.isDir()) {
            m_searchPath = path;

            if (m_searchPath.endsWith("/")) {
                m_searchPath.remove(m_searchPath.size() - 1);
            }
        }
    }
}

QString AssetManager::pathFor(const QString &relativePath) const
{
    const QUrl supposedUrl = QUrl(m_searchPath + "/").resolved(relativePath);
    const QString supposedFilePath = supposedUrl.toString();
    const QFileInfo fileInfo(supposedFilePath);

    return fileInfo.exists() ? supposedFilePath : relativePath;
}

const BigFile &AssetManager::bigFile()
{
    return m_bigFile;
}

void AssetManager::loadBigFile(const std::string &path)
{
    m_bigFile = importBigFile(path);
}
