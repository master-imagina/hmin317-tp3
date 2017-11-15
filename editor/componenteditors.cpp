#include "componenteditors.h"

#include <QFormLayout>
#include <QLabel>

#include "editor/gui/vec3edit.h"

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
