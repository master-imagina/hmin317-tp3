#ifndef COMPONENTEDITORS_H
#define COMPONENTEDITORS_H

#include "3rdparty/entityx/entityx.h"

class QVBoxLayout;
class QWidget;


void createTransformEditor(entityx::Entity entity, QWidget *parent, QVBoxLayout *layout);
void createParticleEffectEditor(entityx::Entity entity, QWidget *parent, QVBoxLayout *layout);


#endif // COMPONENTEDITORS_H
