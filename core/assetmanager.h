#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <unordered_map>

#include <QImage>

#include "core/bigfile.h"


class AssetManager
{
public:
    static AssetManager *self();

    ~AssetManager() = default;

    const BigFile &bigFile();
    void loadBigFile(const std::string &path);

    static QStringList supportedImageFormats();
    QImage *image(const std::string &path);

private:
    AssetManager();

private:
    static AssetManager *m_self;

    BigFile m_bigFile;

    std::unordered_map<std::string, QImage> m_images;
};

#endif // ASSETMANAGER_H
