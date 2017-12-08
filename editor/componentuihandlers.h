#ifndef COMPONENUIHANDLERS_H
#define COMPONENUIHANDLERS_H

#include "icomponentuihandler.h"

#include "input/keyboard.h"

#include "render/camera.h"
#include "render/light.h"
#include "render/mesh.h"
#include "render/transform.h"

#include "render/material/material.h"

#include "extras/particles/particleeffect.h"

#include "script/script.h"

class LuaServer;


class TransformCompUiHandler : public TComponentUiHandler<Transform>
{
public:
    QString componentName() override { return "Transform"; }

    void configureAddAction(entityx::Entity &entity,
                            QAction *action) override;
    void configureRemoveAction(entityx::Entity &entity,
                               QAction *action) override;
    QWidget *createComponentEditor(entityx::Entity entity,
                                   QWidget *parent,
                                   const QString &projectPath) override;
};


class ParticleEffectCompUiHandler : public TComponentUiHandler<ParticleEffect>
{
public:
    QString componentName() override { return "ParticleEffect"; }

    void configureAddAction(entityx::Entity &entity,
                            QAction *action) override;
    void configureRemoveAction(entityx::Entity &entity,
                               QAction *action) override;
    QWidget *createComponentEditor(entityx::Entity entity,
                                   QWidget *parent,
                                   const QString &projectPath) override;
};

class MeshCompUiHandler : public TComponentUiHandler<Mesh>
{
public:
    QString componentName() override { return "Mesh"; }

    void configureAddAction(entityx::Entity &entity,
                            QAction *action) override;
    void configureRemoveAction(entityx::Entity &entity,
                               QAction *action) override;
    QWidget *createComponentEditor(entityx::Entity entity,
                                   QWidget *parent,
                                   const QString &projectPath) override;
};

class LightCompUiHandler : public TComponentUiHandler<Light>
{
public:
    QString componentName() override { return "Light"; }

    void configureAddAction(entityx::Entity &entity,
                            QAction *action) override;
    void configureRemoveAction(entityx::Entity &entity,
                               QAction *action) override;
    QWidget *createComponentEditor(entityx::Entity entity,
                                   QWidget *parent,
                                   const QString &projectPath) override;
};

class MaterialCompUiHandler : public TComponentUiHandler<Material>
{
public:
    QString componentName() override { return "Material"; }

    void configureAddAction(entityx::Entity &entity,
                            QAction *action) override;
    void configureRemoveAction(entityx::Entity &entity,
                               QAction *action) override;
    QWidget *createComponentEditor(entityx::Entity entity,
                                   QWidget *parent,
                                   const QString &projectPath) override;
};

class CameraCompUiHandler : public TComponentUiHandler<Camera>
{
public:
    QString componentName() override { return "Camera"; }

    void configureAddAction(entityx::Entity &entity,
                            QAction *action) override;
    void configureRemoveAction(entityx::Entity &entity,
                               QAction *action) override;
    QWidget *createComponentEditor(entityx::Entity entity,
                                   QWidget *parent,
                                   const QString &projectPath) override;
};

class KeyboardCompUiHandler : public TComponentUiHandler<Keyboard>
{
public:
    QString componentName() override { return "Keyboard"; }

    void configureAddAction(entityx::Entity &entity,
                            QAction *action) override;
    void configureRemoveAction(entityx::Entity &entity,
                               QAction *action) override;
    QWidget *createComponentEditor(entityx::Entity entity,
                                   QWidget *parent,
                                   const QString &projectPath) override;
};

class ScriptCompUiHandler : public TComponentUiHandler<Script>
{
public:
    ScriptCompUiHandler(LuaServer &luaServer);

    QString componentName() override { return "Script"; }

    void configureAddAction(entityx::Entity &entity,
                            QAction *action) override;
    void configureRemoveAction(entityx::Entity &entity,
                               QAction *action) override;
    QWidget *createComponentEditor(entityx::Entity entity,
                                   QWidget *parent,
                                   const QString &projectPath) override;

private:
    LuaServer &m_theLuaServer;
};

#endif // COMPONENUIHANDLERS_H
