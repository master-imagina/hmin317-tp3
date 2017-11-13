#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <unordered_map>

#include "core/aliases_int.h"

class GLWrapper;
class Texture2D;


class TextureManager
{
public:
    static int RESERVE_TEXTURE_COUNT;

    TextureManager();

    uint32 addTexture(Texture2D *texture, GLWrapper &glWrapper);
    bool isAllocated(Texture2D *texture) const;

    uint32 textureIdForTexture(Texture2D *texture) const;

    void cleanup(GLWrapper &glWrapper);

private:
    std::unordered_map<Texture2D *, uint32> m_textureToId;
};

#endif // TEXTUREMANAGER_H
