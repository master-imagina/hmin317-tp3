#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <unordered_map>

#include "core/aliases_memory.h"

#include "render/material/texture.h"

#include "render/renderer/gltexture.h"

class GLWrapper;


class TextureManager
{
public:
    static int RESERVE_TEXTURE_COUNT;

    TextureManager();

    GLTexture *addTexture(const Texture2D &texture, GLWrapper &glWrapper);
    bool isAllocated(const Texture2D &texture) const;

    GLTexture *get(const Texture2D &texture) const;

    void cleanup(GLWrapper &glWrapper);

private:
    std::unordered_map<Texture2D, uptr<GLTexture>> m_textureToId;
};

#endif // TEXTUREMANAGER_H
