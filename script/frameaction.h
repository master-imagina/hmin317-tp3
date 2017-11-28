#ifndef FRAMEACTION_H
#define FRAMEACTION_H

#include <functional>

#include "3rdparty/entityx/Entity.h"


using FrameAction = std::function<void (entityx::Entity &entity, float)>;

#endif // FRAMEACTION_H
