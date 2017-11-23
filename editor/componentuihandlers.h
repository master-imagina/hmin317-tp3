#ifndef COMPONENUIHANDLERS_H
#define COMPONENUIHANDLERS_H

#include "icomponentuihandler.h"

#include "render/mesh.h"
#include "render/transform.h"

#include "extras/particles/particleeffect.h"


class TransformCompUiHandler : public TComponentUiHandler<Transform>
{
public:
    QString componentName() override { return "Transform"; }

    void configureAddAction(entityx::Entity &entity,
                            QAction *action) override;
    void createComponentEditor(entityx::Entity entity,
                               QWidget *parent,
                               QVBoxLayout *layout,
                               const QString &projectPath) override;
};


class ParticleEffectCompUiHandler : public TComponentUiHandler<ParticleEffect>
{
public:
    QString componentName() override { return "ParticleEffect"; }

    void configureAddAction(entityx::Entity &entity,
                            QAction *action) override;
    void createComponentEditor(entityx::Entity entity,
                               QWidget *parent,
                               QVBoxLayout *layout,
                               const QString &projectPath) override;
};

class MeshCompUiHandler : public TComponentUiHandler<Mesh>
{
public:
    QString componentName() override { return "Mesh"; }

    void configureAddAction(entityx::Entity &entity,
                            QAction *action) override;
    void createComponentEditor(entityx::Entity entity,
                               QWidget *parent,
                               QVBoxLayout *layout,
                               const QString &projectPath) override;
};

#endif // COMPONENUIHANDLERS_H
