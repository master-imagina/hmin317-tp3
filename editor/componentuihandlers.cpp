#include "componentuihandlers.h"

#include <string>

#include <QAction>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QUrl>

#include "editor/gui/advancedslider.h"
#include "editor/gui/coloreditor/coloreditor.h"
#include "editor/gui/vec3edit.h"

#include "extras/particles/quick.h"

#include "render/material/renderpass.h"
#include "core/param.h"
#include "render/material/shaderprogram.h"

#include "script/luaserver.h"
#include "script/scriptassets.h"


////////////////////// Helpers //////////////////////

namespace {

QWidget *createParamEditor(Param &param, QWidget *parent)
{
    const QVariant oldValue = param.value;
    const int paramType = oldValue.userType();

    QWidget *ret = nullptr;

    if (paramType == QMetaType::Int) {
        auto *editor = new ValuedSlider(Qt::Horizontal, parent);
        editor->setValue(param.value.toInt());

        QObject::connect(editor, &ValuedSlider::valueChanged,
                         [&param] (int value) { param.value = value; });

        ret = editor;
    }
    else if (paramType == QMetaType::Float) {
        //FIXME
        auto *editor = new ValuedSlider(Qt::Horizontal, parent);
        editor->setValue(param.value.toInt());

        QObject::connect(editor, &ValuedSlider::valueChanged,
                         [&param] (int value) { param.value = static_cast<float>(value); });

        ret = editor;
    }
    else if (paramType == QMetaType::QVector3D) {
        auto *editor = new Vec3DEdit(parent);
        editor->setValue(param.value.value<QVector3D>());

        QObject::connect(editor, &Vec3DEdit::valueChanged,
                         [&param] (const QVector3D &value) { param.value = value; });

        ret = editor;
    }
    else if (paramType == QMetaType::QColor) {
        auto *editor = new ColorEditor(parent);
        editor->setValue(param.value.value<QColor>());

        QObject::connect(editor, &ColorEditor::valueChanged,
                         [&param] (const QColor &value) { param.value = value; });

        ret = editor;
    }

    // Check type support
    if (!ret) {
        std::cout << "[EDITOR - WARNING] createParamEditor(): "
                  << "unsupported parameter type ("
                  << QMetaType::typeName(paramType) << ")"
                  << std::endl;
        ret = new QWidget(parent);
    }

    return ret;
}

} // anon namespace


////////////////////// TransformCompUiHandler //////////////////////

void TransformCompUiHandler::configureAddAction(entityx::Entity &entity,
                                                QAction *action)
{
    QObject::connect(action, &QAction::triggered,
                     [&entity] {
        entity.assign<Transform>();
    });
}

QWidget *TransformCompUiHandler::createComponentEditor(entityx::Entity entity,
                                                       QWidget *parent,
                                                       const QString &)
{
    Transform *comp = entity.component<Transform>().get();

    // Build UI
    auto *ret = new QWidget(parent);

    auto *translateEditor = new Vec3DEdit(parent);
    translateEditor->setValue(comp->translate());

    auto *rotateEditor = new Vec3DEdit(parent);
    rotateEditor->setValue(comp->rotation());
    const float maxRotateAngle = 360.;
    rotateEditor->setXMax(maxRotateAngle);
    rotateEditor->setYMax(maxRotateAngle);
    rotateEditor->setZMax(maxRotateAngle);

    auto *scaleEditor = new Vec3DEdit(parent);
    scaleEditor->setXMin(1.);
    scaleEditor->setYMin(1.);
    scaleEditor->setZMin(1.);
    scaleEditor->setValue(comp->scale());

    auto *editorLayout = new QFormLayout(ret);
    editorLayout->addRow("Translation", translateEditor);
    editorLayout->addRow("Rotation", rotateEditor);
    editorLayout->addRow("Scale", scaleEditor);

    // Create connections
    QObject::connect(translateEditor, &Vec3DEdit::valueChanged,
                     [comp] (const QVector3D &value) {
        comp->setTranslate(value);
    });

    QObject::connect(rotateEditor, &Vec3DEdit::valueChanged,
                     [comp] (const QVector3D &value) {
        comp->setRotation(value);
    });

    QObject::connect(scaleEditor, &Vec3DEdit::valueChanged,
                     [comp] (const QVector3D &value) {
        comp->setScale(value);
    });

    return ret;
}


////////////////////// ParticleEffectCompUiHandler //////////////////////

void ParticleEffectCompUiHandler::configureAddAction(entityx::Entity &entity,
                                                     QAction *action)
{
    QObject::connect(action, &QAction::triggered,
                     [entity] {
        createParticleEffect(entity);
    });
}

QWidget *ParticleEffectCompUiHandler::createComponentEditor(entityx::Entity entity,
                                                            QWidget *parent,
                                                            const QString &)
{
    ParticleEffect *comp = entity.component<ParticleEffect>().get();

    // Build UI
    auto *ret = new QWidget(parent);

    auto *directionEditor = new Vec3DEdit(ret);
    directionEditor->setXMin(-1.f);
    directionEditor->setYMin(-1.f);
    directionEditor->setZMin(-1.f);
    directionEditor->setXMax(1.f);
    directionEditor->setYMax(1.f);
    directionEditor->setZMax(1.f);
    directionEditor->setStepSize(0.1f);
    directionEditor->setValue(comp->direction());
    auto *countSlider = new ValuedSlider(Qt::Horizontal, ret);
    countSlider->setMaximum(500);
    countSlider->setValue(comp->count());
    auto *spawnRateSlider = new ValuedSlider(Qt::Horizontal, ret);
    spawnRateSlider->setMinimum(1);
    spawnRateSlider->setMaximum(100);
    spawnRateSlider->setValue(comp->spawnRate());
    auto *maxLifeSlider = new ValuedSlider(Qt::Horizontal, ret);
    maxLifeSlider->setValue(comp->maxLife());
    auto *radiusSlider = new ValuedSlider(Qt::Horizontal, ret);
    radiusSlider->setValue(comp->radius());
    auto *speedSlider = new ValuedSlider(Qt::Horizontal, ret);
    speedSlider->setValue(comp->speed());
    auto *particleSizeSlider = new ValuedSlider(Qt::Horizontal, ret);
    particleSizeSlider->setValue(comp->particleSize());

    auto *editorLayout = new QFormLayout(ret);
    editorLayout->addRow("Direction", directionEditor);
    editorLayout->addRow("Count", countSlider);
    editorLayout->addRow("Spawn Rate", spawnRateSlider);
    editorLayout->addRow("Max Life", maxLifeSlider);
    editorLayout->addRow("Radius", radiusSlider);
    editorLayout->addRow("Speed", speedSlider);
    editorLayout->addRow("Particle Size", particleSizeSlider);

    QObject::connect(directionEditor, &Vec3DEdit::valueChanged,
                     [comp] (const QVector3D &value) {
        comp->setDirection(value);
    });

    QObject::connect(countSlider, &QSlider::valueChanged,
                     [comp] (int value) {
        comp->setCount(value);
    });

    QObject::connect(spawnRateSlider, &QSlider::valueChanged,
                     [comp] (int value) {
        comp->setSpawnRate(value);
    });

    QObject::connect(maxLifeSlider, &QSlider::valueChanged,
                     [comp] (int value) {
        comp->setMaxLife(value);
    });

    QObject::connect(radiusSlider, &QSlider::valueChanged,
                     [comp] (int value) {
        comp->setRadius(value);
    });

    QObject::connect(speedSlider, &QSlider::valueChanged,
                     [comp] (int value) {
        comp->setSpeed(value);
    });

    QObject::connect(particleSizeSlider, &QSlider::valueChanged,
                     [comp] (int value) {
        comp->setParticleSize(value);
        //FIXME
        //        m_particleMaterial->setParam("particleSize", (float)value);
    });

    return ret;
}


////////////////////// MeshCompUiHandler //////////////////////

void MeshCompUiHandler::configureAddAction(entityx::Entity &entity,
                                           QAction *action)
{
    QObject::connect(action, &QAction::triggered,
                     [&entity] {
        entity.assign<Mesh>();
    });
}

QWidget *MeshCompUiHandler::createComponentEditor(entityx::Entity entity,
                                                  QWidget *parent,
                                                  const QString &projectPath)
{
    Mesh *comp = entity.component<Mesh>().get();

    // Build UI
    auto *ret = new QWidget(parent);

    auto *meshPathEditor = new QLineEdit(ret);
    meshPathEditor->setText(QString::fromStdString(comp->path()));

    auto *editorLayout = new QFormLayout(ret);
    editorLayout->addRow("Path", meshPathEditor);

    // Create connections
    QObject::connect(meshPathEditor, &QLineEdit::editingFinished,
                     [comp, meshPathEditor] {
        comp->setPath(meshPathEditor->text().toStdString());
    });

    return ret;
}


////////////////////// LightCompUiHandler //////////////////////

void LightCompUiHandler::configureAddAction(entityx::Entity &entity,
                                            QAction *action)
{
    QObject::connect(action, &QAction::triggered,
                     [&entity] {
        entity.assign<Light>();
    });
}

QWidget *LightCompUiHandler::createComponentEditor(entityx::Entity entity,
                                                   QWidget *parent,
                                                   const QString &)
{
    Light *comp = entity.component<Light>().get();

    // Build UI
    auto *ret = new QWidget(parent);

    auto *lightColorEditor = new ColorEditor(ret);
    lightColorEditor->setValue(comp->color);

    auto *editorLayout = new QFormLayout(ret);
    editorLayout->addRow("Color", lightColorEditor);

    // Create connections
    QObject::connect(lightColorEditor, &ColorEditor::valueChanged,
                     [comp, lightColorEditor] {
        comp->color = lightColorEditor->value();
    });

    return ret;
}


////////////////////// MaterialCompUiHandler //////////////////////

void MaterialCompUiHandler::configureAddAction(entityx::Entity &entity,
                                               QAction *action)
{
    QObject::connect(action, &QAction::triggered,
                     [&entity] {
        entity.assign<Material>();
    });
}

QWidget *MaterialCompUiHandler::createComponentEditor(entityx::Entity entity,
                                                      QWidget *parent,
                                                      const QString &projectPath)
{
    Material *comp = entity.component<Material>().get();

    // Build UI
    auto *ret = new QWidget(parent);

    auto *editorLayout = new QFormLayout(ret);

    for (Param &param : comp->params()) {
        QWidget *paramEditor = createParamEditor(param, ret);

        editorLayout->addRow(QString::fromStdString(param.name), paramEditor);
    }

    return ret;
}


////////////////////// CameraCompUiHandler //////////////////////

void CameraCompUiHandler::configureAddAction(entityx::Entity &entity,
                                             QAction *action)
{
    QObject::connect(action, &QAction::triggered,
                     [&entity] {
        entity.assign<Camera>();
    });
}

QWidget *CameraCompUiHandler::createComponentEditor(entityx::Entity entity,
                                                    QWidget *parent,
                                                    const QString &projectPath)
{
    Camera *comp = entity.component<Camera>().get();

    // Build UI
    auto *ret = new QWidget(parent);

    auto *eyePosEditor = new Vec3DEdit(ret);
    eyePosEditor->setValue(comp->eyePos());

    QObject::connect(eyePosEditor, &Vec3DEdit::valueChanged,
                     [comp] (const QVector3D &value) {
        comp->setEyePos(value);
    });

    auto *targetPosEditor = new Vec3DEdit(ret);
    targetPosEditor->setValue(comp->targetPos());

    QObject::connect(targetPosEditor, &Vec3DEdit::valueChanged,
                     [comp] (const QVector3D &value) {
        comp->setTargetPos(value);
    });


    auto *editorLayout = new QFormLayout(ret);
    editorLayout->addRow("Eye Pos", eyePosEditor);
    editorLayout->addRow("Target Pos", targetPosEditor);

    return ret;
}


////////////////////// KeyboardCompUiHandler //////////////////////

void KeyboardCompUiHandler::configureAddAction(entityx::Entity &entity,
                                               QAction *action)
{
    QObject::connect(action, &QAction::triggered,
                     [&entity] {
        entity.assign<Keyboard>();
    });
}

QWidget *KeyboardCompUiHandler::createComponentEditor(entityx::Entity entity,
                                                      QWidget *parent,
                                                      const QString &projectPath)
{
    Keyboard *comp = entity.component<Keyboard>().get();

    // Build UI
    auto *ret = new QWidget(parent);

    return ret;
}


////////////////////// ScriptCompUiHandler //////////////////////

ScriptCompUiHandler::ScriptCompUiHandler(LuaServer &luaServer) :
    m_theLuaServer(luaServer)
{}

void ScriptCompUiHandler::configureAddAction(entityx::Entity &entity,
                                             QAction *action)
{
    QObject::connect(action, &QAction::triggered,
                     [&entity] {
        entity.assign<Script>();
    });
}

QWidget *ScriptCompUiHandler::createComponentEditor(entityx::Entity entity,
                                                    QWidget *parent,
                                                    const QString &projectPath)
{
    Script *comp = entity.component<Script>().get();

    // Build UI
    auto *ret = new QWidget(parent);

    auto *scriptPathEditor = new QLineEdit(ret);
    scriptPathEditor->setText(QString::fromStdString(comp->path));

    auto *editorLayout = new QFormLayout(ret);
    editorLayout->addRow("Path", scriptPathEditor);

    for (Param &param: comp->params()) {
        editorLayout->addRow(QString::fromStdString(param.name),
                             createParamEditor(param, parent));
    }

    // Create connections
    QObject::connect(scriptPathEditor, &QLineEdit::editingFinished,
                     [this, comp, scriptPathEditor] {
        *comp = scriptFromFile(scriptPathEditor->text().toStdString(),
                               m_theLuaServer);
    });

    return ret;
}
