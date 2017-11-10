#include "particleeditor.h"

#include <QVBoxLayout>

#include "extras/cameraactions.h"
#include "extras/gamewidget.h"
#include "extras/heightmap.h"

#include "extras/particles/quick.h"

#include "render/aabb.h"

#include "render/geometry/geometry.h"

#include "render/material/material.h"
#include "render/material/renderpass.h"
#include "render/material/shaderutils.h"


ParticleEditor::ParticleEditor(QWidget *parent) :
    QWidget(parent),
    m_scene(),
    m_camera(),
    m_gameWidget(nullptr),
    m_particleEffect(),
    m_particleMaterial()
{
    m_gameWidget = new GameWidget(m_scene, this);
    m_gameWidget->setCamera(&m_camera);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_gameWidget);

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
    uptr<ShaderProgram> terrainShader = shaderProgramFromFile("://res/shaders/terrain_heightmap.vert",
                                                              "",
                                                              "://res/shaders/terrain_wireframe.frag");
    terrainPass->setShaderProgram(std::move(terrainShader));

    // Create particle effect
    entityx::Entity particleEntity =
            createParticleEffect(m_scene, {0, 400, 0}, {0, 1, 0},
                                 50, 100, gridBBox.radius().z(),
                                 0.4f, 4.f);

    m_particleEffect = particleEntity.component<ParticleEffect>();
    m_particleMaterial = particleEntity.component<Material>();

    // Center camera above terrain
    centerCameraOnBBox(m_camera, gridBBox);
}
