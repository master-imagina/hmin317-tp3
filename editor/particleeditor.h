#ifndef PARTICLEEDITOR_H
#define PARTICLEEDITOR_H

#include <QWidget>

#include <entityx/Entity.h>

#include "core/scene.h"

#include "extras/particles/particleeffect.h"

#include "render/material/material.h"


class GameWidget;


class ParticleEditor : public QWidget
{
public:
    ParticleEditor(QWidget *parent = nullptr);

private:
    void initEditorScene();
    void initGui();

private:
    Scene m_scene;

    GameWidget *m_gameWidget;

    entityx::ComponentHandle<ParticleEffect> m_particleEffect;
    entityx::ComponentHandle<Material> m_particleMaterial;
};

#endif // PARTICLEEDITOR_H
