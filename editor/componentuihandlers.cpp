#include "componentuihandlers.h"

#include <QAction>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QUrl>

#include "editor/gui/advancedslider.h"
#include "editor/gui/coloreditor/coloreditor.h"
#include "editor/gui/vec3edit.h"

#include "extras/particles/quick.h"


////////////////////// TransformCompUiHandler //////////////////////

void TransformCompUiHandler::configureAddAction(entityx::Entity &entity,
                                                QAction *action)
{
    QObject::connect(action, &QAction::triggered,
                     [&entity] {
        entity.assign<Transform>();
    });
}

void TransformCompUiHandler::createComponentEditor(entityx::Entity entity,
                                                   QWidget *parent,
                                                   QVBoxLayout *layout,
                                                   const QString &)
{
    Transform *comp = entity.component<Transform>().get();

    // Build UI
    auto *editorWidget = new QWidget(parent);

    layout->insertWidget(layout->count() - 1, new QLabel(componentName()));
    layout->insertWidget(layout->count() - 1, editorWidget);


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

    auto *editorLayout = new QFormLayout(editorWidget);
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

void ParticleEffectCompUiHandler::createComponentEditor(entityx::Entity entity,
                                                        QWidget *parent,
                                                        QVBoxLayout *layout,
                                                        const QString &)
{
    ParticleEffect *comp = entity.component<ParticleEffect>().get();

    // Build UI
    auto *editorWidget = new QWidget(parent);

    layout->insertWidget(layout->count() - 1, new QLabel(componentName()));
    layout->insertWidget(layout->count() - 1, editorWidget);

    auto *directionEditor = new Vec3DEdit(editorWidget);
    directionEditor->setXMin(-1.f);
    directionEditor->setYMin(-1.f);
    directionEditor->setZMin(-1.f);
    directionEditor->setXMax(1.f);
    directionEditor->setYMax(1.f);
    directionEditor->setZMax(1.f);
    directionEditor->setStepSize(0.1f);
    directionEditor->setValue(comp->direction());
    auto *countSlider = new ValuedSlider(Qt::Horizontal, editorWidget);
    countSlider->setMaximum(500);
    countSlider->setValue(comp->count());
    auto *spawnRateSlider = new ValuedSlider(Qt::Horizontal, editorWidget);
    spawnRateSlider->setMinimum(1);
    spawnRateSlider->setMaximum(100);
    spawnRateSlider->setValue(comp->spawnRate());
    auto *maxLifeSlider = new ValuedSlider(Qt::Horizontal, editorWidget);
    maxLifeSlider->setValue(comp->maxLife());
    auto *radiusSlider = new ValuedSlider(Qt::Horizontal, editorWidget);
    radiusSlider->setValue(comp->radius());
    auto *speedSlider = new ValuedSlider(Qt::Horizontal, editorWidget);
    speedSlider->setValue(comp->speed());
    auto *particleSizeSlider = new ValuedSlider(Qt::Horizontal, editorWidget);
    particleSizeSlider->setValue(comp->particleSize());

    auto *editorLayout = new QFormLayout(editorWidget);
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

void MeshCompUiHandler::createComponentEditor(entityx::Entity entity,
                                              QWidget *parent,
                                              QVBoxLayout *layout,
                                              const QString &projectPath)
{
    Mesh *comp = entity.component<Mesh>().get();

    // Build UI
    auto *editorWidget = new QWidget(parent);

    layout->insertWidget(layout->count() - 1, new QLabel(componentName()));
    layout->insertWidget(layout->count() - 1, editorWidget);

    auto *meshPathEditor = new QLineEdit(editorWidget);
    meshPathEditor->setText(QString::fromStdString(comp->path()));

    auto *editorLayout = new QFormLayout(editorWidget);
    editorLayout->addRow("Path", meshPathEditor);

    // Create connections
    QObject::connect(meshPathEditor, &QLineEdit::editingFinished,
                     [comp, meshPathEditor, projectPath] {
        comp->setPath(meshPathEditor->text().toStdString());
    });
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

void LightCompUiHandler::createComponentEditor(entityx::Entity entity,
                                               QWidget *parent,
                                               QVBoxLayout *layout,
                                               const QString &)
{
    Light *comp = entity.component<Light>().get();

    // Build UI
    auto *editorWidget = new QWidget(parent);

    layout->insertWidget(layout->count() - 1, new QLabel(componentName()));
    layout->insertWidget(layout->count() - 1, editorWidget);

    auto *lightColorEditor = new ColorEditor(editorWidget);
    lightColorEditor->setValue(comp->color);

    auto *editorLayout = new QFormLayout(editorWidget);
    editorLayout->addRow("Color", lightColorEditor);

    // Create connections
    QObject::connect(lightColorEditor, &ColorEditor::valueChanged,
                     [comp, lightColorEditor] {
        comp->color = lightColorEditor->value();
    });
}
