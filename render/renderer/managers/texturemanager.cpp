#include "texturemanager.h"

#include <assert.h>
#include <iostream>

#include "core/assetmanager.h"

#include "render/renderassets.h"

#include "render/material/texture.h"

#include "render/renderer/gltexture.h"
#include "render/renderer/glwrapper.h"


int TextureManager::RESERVE_TEXTURE_COUNT = 64;

TextureManager::TextureManager() :
    m_textureToId()
{
    m_textureToId.reserve(RESERVE_TEXTURE_COUNT);
}

GLTexture *TextureManager::addTexture(const Texture2D &texture, GLWrapper &glWrapper)
{
    const auto isHandled = m_textureToId.find(texture);

    if (isHandled != m_textureToId.cend()) {
        std::cout << "[WARNING] TextureManager::addTexture(): texture already exists"
                  << std::endl;
        return nullptr;
    }

    // Read texture from file
    QImage image = imageFromFile(texture.path);

    if (image.isNull()) {
        std::cerr << "TextureManager::addTexture(): "
                  << texture.path << " does not exist" << std::endl;

        return nullptr;
    }

    image = image.convertToFormat(QImage::Format_RGBA8888);

    auto glTexture = std::make_unique<GLTexture>();
    GLTexture *ret = glTexture.get();
    m_textures.emplace_back(std::move(glTexture));

    switch (image.pixelFormat().channelCount()) {
    case 1:
        ret->params.format = GLTexture::Params::Format::Red;
        break;
    case 3:
        ret->params.format = GLTexture::Params::Format::Rgb;
        break;
    case 4:
        ret->params.format = GLTexture::Params::Format::Rgba;
        break;
    default:
        assert (false);
    }

    ret->params.width = image.width();
    ret->params.height = image.height();

    // Allocate GL texture
    glWrapper.createTexture2D(*ret);
    glWrapper.allocateTexture2D(*ret, ret->params, image.constBits());

    m_textureToId.insert({texture, ret});

    return ret;
}

bool TextureManager::isAllocated(const Texture2D &texture) const
{
    const GLTexture *glTexture = get(texture);

    return glTexture != nullptr;
}

GLTexture *TextureManager::get(const Texture2D &texture) const
{
    const auto textureFound = m_textureToId.find(texture);

    GLTexture * ret = nullptr;

    if (textureFound != m_textureToId.end()) {
        ret = textureFound->second;
    }

    return ret;
}

void TextureManager::cleanup(GLWrapper &glWrapper)
{
    for (auto &texture : m_textures) {
        glWrapper.destroyTexture2D(*texture.get());
    }
}
