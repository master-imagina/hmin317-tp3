#include "particleeditor.h"

#include <QFormLayout>
#include <QScrollArea>
#include <QSlider>
#include <QVBoxLayout>

#include "extras/cameraactions.h"
#include "extras/gamewidget.h"
#include "extras/heightmap.h"

#include "extras/particles/quick.h"

#include "gui/fpswidgets.h"
#include "gui/vec3edit.h"

#include "render/aabb.h"

#include "render/geometry/geometry.h"

#include "render/material/material.h"
#include "render/material/renderpass.h"
#include "render/material/shaderparam.h"
#include "render/material/shaderutils.h"


ParticleEditor::ParticleEditor(QWidget *parent) :
    QWidget(parent),
    m_scene(),
    m_camera(),
    m_gameWidget(nullptr),
    m_particleEffect(),
    m_particleMaterial()
{
    initEditorScene();
    initGui();
}

void ParticleEditor::initEditorScene()
{
    m_gameWidget = new GameWidget(m_scene, this);

    // Create particle scene
    entityx::Entity gridEntity = m_scene.createEntity();

    //  Grid geometry
    auto gridGeom = gridEntity.assign<Geometry>();
    *gridGeom.get() = heightmapToGeometry(QImage("images/flat_heightmap.png"));

    AABoundingBox gridBBox(gridGeom->vertices);

    VertexAttrib standardVertexAttrib {"vertexPos", 3, VertexAttrib::Type::Float, false, 0};
    gridGeom->vertexLayout.addAttribute(standardVertexAttrib);

    //  Terrain material
    auto gridMaterial = gridEntity.assign<Material>();
    RenderPass *terrainPass = gridMaterial->addRenderPass("base");
    uptr<ShaderProgram> terrainShader = shaderProgramFromFile("shaders/terrain_heightmap.vert",
                                                              "",
                                                              "shaders/terrain_wireframe.frag");
    terrainPass->setShaderProgram(std::move(terrainShader));

    // Create particle effect
    entityx::Entity particleEntity =
            createParticleEffect(m_scene, {0, 0, 0}, {0, 1, 0},
                                 50, 100, gridBBox.radius().z(),
                                 0.4f, 4.f);

    m_particleEffect = particleEntity.component<ParticleEffect>();
    m_particleMaterial = particleEntity.component<Material>();

    // Center camera above terrain
    centerCameraOnBBox(m_camera, gridBBox);
}

void ParticleEditor::initGui()
{
    m_gameWidget->setFocusPolicy(Qt::StrongFocus);
    m_gameWidget->setCamera(&m_camera);

    createFpsLabel(m_gameWidget->gameLoop(), m_gameWidget);

    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    // Placeholder for properties
    auto *propertiesWidget = new QWidget(scrollArea);
    auto *propertiesLayout = new QFormLayout(propertiesWidget);

    auto *countSlider = new QSlider(Qt::Horizontal, propertiesWidget);
    countSlider->setValue(m_particleEffect->count());
    auto *maxLifeSlider = new QSlider(Qt::Horizontal, propertiesWidget);
    maxLifeSlider->setValue(m_particleEffect->maxLife());
    auto *worldPosEditor = new Vec3DEdit(propertiesWidget);
    auto *radiusSlider = new QSlider(Qt::Horizontal, propertiesWidget);
    radiusSlider->setValue(m_particleEffect->radius());
    auto *directionEditor = new Vec3DEdit(propertiesWidget);
    auto *speedSlider = new QSlider(Qt::Horizontal, propertiesWidget);
    speedSlider->setValue(m_particleEffect->speed());
    auto *particleSizeSlider = new QSlider(Qt::Horizontal, propertiesWidget);
    particleSizeSlider->setValue(m_particleEffect->particleSize());

    connect(countSlider, &QSlider::valueChanged,
            [this] (int value) {
        m_particleEffect->setCount(value);
    });
    connect(maxLifeSlider, &QSlider::valueChanged,
            [this] (int value) {
        m_particleEffect->setMaxLife(value);
    });
    connect(worldPosEditor, &Vec3DEdit::valueChanged,
            [this] (const QVector3D &value) {
        m_particleEffect->setWorldPos(value);
    });
    connect(radiusSlider, &QSlider::valueChanged,
            [this] (int value) {
        m_particleEffect->setRadius(value);
    });
    connect(directionEditor, &Vec3DEdit::valueChanged,
            [this] (const QVector3D &value) {
        m_particleEffect->setDirection(value);
    });
    connect(speedSlider, &QSlider::valueChanged,
            [this] (int value) {
        m_particleEffect->setSpeed(value);
    });
    connect(particleSizeSlider, &QSlider::valueChanged,
            [this] (int value) {
        m_particleEffect->setParticleSize(value);
        m_particleMaterial->setParam("particleSize", (float)value);
    });

    propertiesLayout->addRow("Count", countSlider);
    propertiesLayout->addRow("Max Life", maxLifeSlider);
    propertiesLayout->addRow("World Position", worldPosEditor);
    propertiesLayout->addRow("Radius", radiusSlider);
    propertiesLayout->addRow("Direction", directionEditor);
    propertiesLayout->addRow("Speed", speedSlider);
    propertiesLayout->addRow("Particle Size", particleSizeSlider);

    scrollArea->setWidget(propertiesWidget);

    // Assemble
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_gameWidget, 1);
    mainLayout->addWidget(scrollArea);
}
