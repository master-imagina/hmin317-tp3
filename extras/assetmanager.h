#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <array>
#include <string>
#include <unordered_map>

#include <QImage>

#include "core/aliases_memory.h"

#include "extras/bigfile.h"

#include "render/material/shaderprogram.h"


class AssetManager
{
public:
    static AssetManager *self();

    ~AssetManager() = default;

    const BigFile &bigFile();
    void loadBigFile(const std::string &path);

    static QStringList supportedImageFormats();
    QImage image(const std::string &path);

    uptr<ShaderProgram> shader(const std::string &vertShaderPath,
                               const std::string &geomShaderPath,
                               const std::string &fragShaderPath);

private:
    AssetManager();

private:
    static AssetManager *m_self;

    BigFile m_bigFile;
};

#endif // ASSETMANAGER_H
