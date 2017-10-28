#include "gamewidget.h"

#include <QMatrix4x4>
#include <QtMath>

#include "camera.h"
#include "geometry.h"
#include "particleeffect.h"
#include "scene.h"

#include "renderer/renderer.h"


namespace {

QColor colorFromSeason(Season season)
{
    QColor ret;

    switch (season) {
    case Season::Autumn:
        ret = QColor(244, 183, 51);
        break;
    case Season::Winter:
        ret = Qt::white;
        break;
    case Season::Spring:
        ret = Qt::green;
        break;
    case Season::Summer:
        ret = Qt::yellow;
        break;
    default:
        break;
    }

    return ret;
}

} // anon namespace


GameWidget::GameWidget(Scene *scene, QWidget *parent) :
    QOpenGLWidget(parent),
    m_scene(scene),
    m_renderer(std::make_unique<Renderer>()),
    m_camera(nullptr),
    m_currentSeason(Season::None)
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

void GameWidget::setSeason(Season season)
{
    if (m_currentSeason != season) {
        m_currentSeason = season;
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

    const QColor drawColor = colorFromSeason(m_currentSeason);

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
