#ifndef TEXTUREUTILS_H
#define TEXTUREUTILS_H

#include "texture.h"

#include "core/aliases_memory.h"


uptr<Texture2D> textureFromFile(const std::string &path);

#endif // TEXTUREUTILS_H
