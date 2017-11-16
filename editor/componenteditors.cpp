#include "componenteditors.h"

#include <QFormLayout>
#include <QLabel>

#include "editor/gui/advancedslider.h"
#include "editor/gui/vec3edit.h"

#include "extras/particles/particleeffect.h"

#include "render/transform.h"



void createTransformEditor(entityx::Entity entity,
                           QWidget *parent,
                           QVBoxLayout *layout)
{
    if (!entity.has_component<Transform>()) {
        return;
    }

    Transform *comp = entity.component<Transform>().get();

    // Build UI
    auto *editorWidget = new QWidget(parent);

    layout->insertWidget(layout->count() - 1, new QLabel("Transform"));
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


void createParticleEffectEditor(entityx::Entity entity,
                                QWidget *parent,
                                QVBoxLayout *layout)
{
    if (!entity.has_component<ParticleEffect>()) {
        return;
    }

    ParticleEffect *comp = entity.component<ParticleEffect>().get();

    // Build UI
    auto *editorWidget = new QWidget(parent);

    layout->insertWidget(layout->count() - 1, new QLabel("Particle Effect"));
    layout->insertWidget(layout->count() - 1, editorWidget);


    auto *countSlider = new ValuedSlider(Qt::Horizontal, editorWidget);
    countSlider->setMaximum(500);
    countSlider->setValue(comp->count());
    auto *spawnRateSlider = new ValuedSlider(Qt::Horizontal, editorWidget);
    spawnRateSlider->setMinimum(1);
    spawnRateSlider->setMaximum(100);
    spawnRateSlider->setValue(comp->spawnRate());
    auto *maxLifeSlider = new ValuedSlider(Qt::Horizontal, editorWidget);
    maxLifeSlider->setValue(comp->maxLife());
    auto *worldPosEditor = new Vec3DEdit(editorWidget);
    auto *radiusSlider = new ValuedSlider(Qt::Horizontal, editorWidget);
    radiusSlider->setValue(comp->radius());
    auto *directionEditor = new Vec3DEdit(editorWidget);
    directionEditor->setXMin(-1.f);
    directionEditor->setYMin(-1.f);
    directionEditor->setZMin(-1.f);
    directionEditor->setXMax(1.f);
    directionEditor->setYMax(1.f);
    directionEditor->setZMax(1.f);
    directionEditor->setStepSize(0.1f);
    auto *speedSlider = new ValuedSlider(Qt::Horizontal, editorWidget);
    speedSlider->setValue(comp->speed());
    auto *particleSizeSlider = new ValuedSlider(Qt::Horizontal, editorWidget);
    particleSizeSlider->setValue(comp->particleSize());

    auto *editorLayout = new QFormLayout(editorWidget);
    editorLayout->addRow("Count", countSlider);
    editorLayout->addRow("Spawn Rate", spawnRateSlider);
    editorLayout->addRow("Max Life", maxLifeSlider);
    editorLayout->addRow("World Position", worldPosEditor);
    editorLayout->addRow("Radius", radiusSlider);
    editorLayout->addRow("Direction", directionEditor);
    editorLayout->addRow("Speed", speedSlider);
    editorLayout->addRow("Particle Size", particleSizeSlider);


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

    QObject::connect(worldPosEditor, &Vec3DEdit::valueChanged,
                     [comp] (const QVector3D &value) {
        comp->setWorldPos(value);
    });

    QObject::connect(radiusSlider, &QSlider::valueChanged,
                     [comp] (int value) {
        comp->setRadius(value);
    });

    QObject::connect(directionEditor, &Vec3DEdit::valueChanged,
                     [comp] (const QVector3D &value) {
        comp->setDirection(value);
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
