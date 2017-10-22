#ifndef GLOBAL_H
#define GLOBAL_H

#include <memory>
#include <mutex>
#include <QOpenGLTexture>

class GameObject;

extern std::mutex res_mutex;

extern std::shared_ptr<QOpenGLTexture> * textureTerrain;
extern std::shared_ptr<QOpenGLTexture> * textureSnow;
extern std::shared_ptr<QOpenGLTexture> * textureRain;

extern GameObject * terrain;
extern GameObject * snow;
extern GameObject * rain;

#endif // GLOBAL_H
