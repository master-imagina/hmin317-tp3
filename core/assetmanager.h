#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>

#include "core/bigfile.h"


class AssetManager
{
public:
    static AssetManager *self();
    ~AssetManager() = default;

    const BigFile &bigFile();
    void loadBigFile(const std::string &path);

private:
    AssetManager();

private:
    static AssetManager *m_self;

    BigFile m_bigFile;
};

#endif // ASSETMANAGER_H
