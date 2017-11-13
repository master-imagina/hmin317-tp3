#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <unordered_map>

#include "core/aliases_memory.h"

#include "../../material/texture.h"

#include "../gltexture.h"

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
    uptr_vector<GLTexture> m_textures;
    std::unordered_map<Texture2D, GLTexture *> m_textureToId;
};

#endif // TEXTUREMANAGER_H
