#include "texturemanager.h"

#include <assert.h>
#include <iostream>

#include "../material/texture.h"

#include "glwrapper.h"


int TextureManager::RESERVE_TEXTURE_COUNT = 64;

TextureManager::TextureManager() :
    m_textureToId()
{
    m_textureToId.reserve(RESERVE_TEXTURE_COUNT);
}

uint32 TextureManager::addTexture(Texture2D *texture, GLWrapper &glWrapper)
{
    const auto isHandled = m_textureToId.find(texture);

    if (isHandled != m_textureToId.cend()) {
        std::cerr << "TextureManager::addTexture(): vao already exists" << std::endl;
        return 0;
    }

    uint32 ret = glWrapper.createTexture2D();
    glWrapper.allocateTexture2D(ret, texture->params, texture->image.constBits());

    m_textureToId.insert({texture, ret});

    return ret;
}

bool TextureManager::isAllocated(Texture2D *texture) const
{
    const uint32 textureId = textureIdForTexture(texture);

    return textureId > 0;
}

uint32 TextureManager::textureIdForTexture(Texture2D *texture) const
{
    const auto textureFound = m_textureToId.find(texture);

    uint32 ret = 0;

    if (textureFound != m_textureToId.end()) {
        ret = textureFound->second;
    }

    return ret;
}

void TextureManager::cleanup(GLWrapper &glWrapper)
{
    for (auto &textureAndId : m_textureToId) {
        glWrapper.destroyTexture2D(textureAndId.second);
    }
}
