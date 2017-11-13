#ifndef PARTICLEEDITOR_H
#define PARTICLEEDITOR_H

#include <QWidget>

#include "3rdparty/entityx/Entity.h"

#include "core/scene.h"

#include "render/material/material.h"

#include "render/camera.h"

#include "extras/particles/particleeffect.h"

class GameWidget;
class UrlEdit;


class ParticleEditor : public QWidget
{
public:
    ParticleEditor(QWidget *parent = nullptr);

private Q_SLOTS:
    void setTextureEnabled(bool enable);

private:
    void initEditorScene();
    void initGui();

private:
    Scene m_scene;
    Camera m_camera;
    GameWidget *m_gameWidget;

    entityx::ComponentHandle<ParticleEffect> m_particleEffect;
    entityx::ComponentHandle<Material> m_particleMaterial;

    UrlEdit *m_textureSelectorWidget;
};

#endif // PARTICLEEDITOR_H
