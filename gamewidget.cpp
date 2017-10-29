#include "gamewidget.h"

#include <QMatrix4x4>

#include "aabb.h"
#include "camera.h"
#include "seasoncontroller.h"
#include "scene.h"

#include "renderer/renderer.h"


GameWidget::GameWidget(Scene *scene, SeasonController *seasons, QWidget *parent) :
    QOpenGLWidget(parent),
    m_scene(scene),
    m_seasons(seasons),
    m_renderer(std::make_unique<Renderer>()),
    m_camera(nullptr)
{}

GameWidget::~GameWidget()
{
    // Make sure the context is current before freeing allocated resources
    makeCurrent();

    m_renderer->cleanup();

    doneCurrent();
}

Camera *GameWidget::camera() const
{
    return m_camera;
}

void GameWidget::setCamera(Camera *camera)
{
    if (m_camera != camera) {
        m_camera = camera;
    }
}

void GameWidget::startNewFrame(float dt)
{
    m_deltaTime = dt;

    update();
}

void GameWidget::initializeGL()
{
    m_renderer->initialize();
}

void GameWidget::resizeGL(int w, int h)
{
    m_camera->setAspectRatio(qreal(w) / qreal(h ? h : 1.f));
}

void GameWidget::paintGL()
{
    const QMatrix4x4 viewMatrix = m_camera->viewMatrix();
    const QMatrix4x4 projectionMatrix = m_camera->projectionMatrix();
    const QMatrix4x4 worldMatrix = projectionMatrix * viewMatrix;

    const QVector3D terrainAABBCenter = m_scene->terrainBoundingBox.center();
    const QVector3D terrainAABBRadius = m_scene->terrainBoundingBox.radius();

    const float minHeight = terrainAABBCenter.y() - terrainAABBRadius.y();
    const float maxHeight = terrainAABBCenter.y() + terrainAABBRadius.y();

    const QColor drawColor = m_seasons->colorFromSeason(objectName().toInt());

    //TODO helper classes for uniforms ; don't send uniforms every frame
    QVariantMap uniforms {
        {"viewMatrix", viewMatrix},
        {"projectionMatrix", projectionMatrix},
        {"worldMatrix", worldMatrix},
        {"minHeight", minHeight},
        {"maxHeight", maxHeight},
        {"terrainColor", drawColor},
        {"particleColor", drawColor},
        {"particlesSize", 4.f}
    };

    m_renderer->updateUniforms(uniforms);

    m_renderer->render(m_scene, m_deltaTime);
}
