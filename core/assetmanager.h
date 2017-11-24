#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>

#include <QString>

#include "core/bigfile.h"


class AssetManager
{
public:
    static AssetManager *self();
    ~AssetManager() = default;

    QString searchPath() const;
    void setSearchPath(const QString &path);
    QString pathFor(const QString &relativePath) const;

    const BigFile &bigFile();
    void loadBigFile(const std::string &path);

private:
    AssetManager();

private:
    static AssetManager *m_self;

    QString m_searchPath;

    BigFile m_bigFile;
};

#endif // ASSETMANAGER_H
