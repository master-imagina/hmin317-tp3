#include "particleeditor.h"

#include <QCheckBox>
#include <QFile>
#include <QFormLayout>
#include <QScrollArea>
#include <QVBoxLayout>

#include "extras/cameraactions.h"
#include "extras/gamewidget.h"
#include "extras/grid.h"

#include "extras/particles/quick.h"

#include "gui/coloreditor/coloreditor.h"

#include "gui/advancedslider.h"
#include "gui/fpswidgets.h"
#include "gui/urledit.h"
#include "gui/vec3edit.h"

#include "render/aabb.h"

#include "render/geometry/geometry.h"

#include "render/material/material.h"
#include "render/material/renderpass.h"
#include "render/material/shaderparam.h"
#include "render/material/shaderutils.h"
#include "render/material/texture.h"



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
    *gridGeom.get() = grid(50);

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
    entityx::Entity particleEntity = m_scene.createEntity();
    createParticleEffect(particleEntity, {0, 0, 0}, {0, 1, 0},
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

    auto *directionEditor = new Vec3DEdit(propertiesWidget);
    directionEditor->setXMin(-1.f);
    directionEditor->setYMin(-1.f);
    directionEditor->setZMin(-1.f);
    directionEditor->setXMax(1.f);
    directionEditor->setYMax(1.f);
    directionEditor->setZMax(1.f);
    directionEditor->setStepSize(0.1f);
    directionEditor->setValue(m_particleEffect->direction());
    auto *countSlider = new ValuedSlider(Qt::Horizontal, propertiesWidget);
    countSlider->setMaximum(500);
    countSlider->setValue(m_particleEffect->count());
    auto *spawnRateSlider = new ValuedSlider(Qt::Horizontal, propertiesWidget);
    spawnRateSlider->setMinimum(1);
    spawnRateSlider->setMaximum(100);
    spawnRateSlider->setValue(m_particleEffect->spawnRate());
    auto *maxLifeSlider = new ValuedSlider(Qt::Horizontal, propertiesWidget);
    maxLifeSlider->setValue(m_particleEffect->maxLife());
    auto *radiusSlider = new ValuedSlider(Qt::Horizontal, propertiesWidget);
    radiusSlider->setValue(m_particleEffect->radius());
    auto *speedSlider = new ValuedSlider(Qt::Horizontal, propertiesWidget);
    speedSlider->setValue(m_particleEffect->speed());
    auto *particleSizeSlider = new ValuedSlider(Qt::Horizontal, propertiesWidget);
    particleSizeSlider->setValue(m_particleEffect->particleSize());

    auto *particleColorEditor = new ColorEditor(propertiesWidget);
    particleColorEditor->setValue(m_particleMaterial->param("particleColor")->value.value<QColor>());

    auto *fullTextureSelectorWidget = new QWidget(propertiesWidget);
    auto *textureSelectorLayout = new QHBoxLayout(fullTextureSelectorWidget);
    textureSelectorLayout->setMargin(0);
    auto *textureSelectorWidget = new UrlEdit(fullTextureSelectorWidget);
    textureSelectorWidget->setNameFilters({"Image files (*.png *.jpg)"});
    textureSelectorWidget->setUrl(QString::fromStdString(m_particleMaterial->param("particleTexture")->value.value<Texture2D>().path));
    auto *enableTextureCheckBox = new QCheckBox(fullTextureSelectorWidget);
    enableTextureCheckBox->setChecked(true);
    connect(enableTextureCheckBox, &QCheckBox::toggled,
            [this, textureSelectorWidget] (bool enable) {
        textureSelectorWidget->setEnabled(enable);

        // Update the shader flag
        const float textureFlag = (enable) ? 1.f : 0.f;

        m_particleMaterial->setParam("textureFlag", textureFlag);
    });

    textureSelectorLayout->addWidget(textureSelectorWidget);
    textureSelectorLayout->addWidget(enableTextureCheckBox);

    connect(directionEditor, &Vec3DEdit::valueChanged,
            [this] (const QVector3D &value) {
        m_particleEffect->setDirection(value);
    });

    connect(countSlider, &QSlider::valueChanged,
            [this] (int value) {
        m_particleEffect->setCount(value);
    });

    connect(spawnRateSlider, &QSlider::valueChanged,
            [this] (int value) {
        m_particleEffect->setSpawnRate(value);
    });

    connect(maxLifeSlider, &QSlider::valueChanged,
            [this] (int value) {
        m_particleEffect->setMaxLife(value);
    });

    connect(radiusSlider, &QSlider::valueChanged,
            [this] (int value) {
        m_particleEffect->setRadius(value);
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

    connect(particleColorEditor, &ColorEditor::valueChanged,
            [this] (const QColor &value) {
        m_particleMaterial->setParam("particleColor", value);
    });

    connect(textureSelectorWidget, &UrlEdit::urlChanged,
            [this] (const QUrl &url) {
        const QString path = url.toString(QUrl::PreferLocalFile);

        const float textureFlag = (QFile::exists(path)) ? 1.f : 0.f;

        if (textureFlag) {
            Texture2D texture { path.toStdString() };
            m_particleMaterial->setParam("particleTexture", QVariant::fromValue(texture));
        }

        m_particleMaterial->setParam("textureFlag", textureFlag);
    });

    propertiesLayout->addRow("Direction", directionEditor);
    propertiesLayout->addRow("Count", countSlider);
    propertiesLayout->addRow("Spawn Rate", spawnRateSlider);
    propertiesLayout->addRow("Max Life", maxLifeSlider);
    propertiesLayout->addRow("Radius", radiusSlider);
    propertiesLayout->addRow("Speed", speedSlider);
    propertiesLayout->addRow("Particle Size", particleSizeSlider);
    propertiesLayout->addRow("Particle Color", particleColorEditor);
    propertiesLayout->addRow("Particle Texture", fullTextureSelectorWidget);

    scrollArea->setWidget(propertiesWidget);

    // Assemble
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_gameWidget, 1);
    mainLayout->addWidget(scrollArea);
}
