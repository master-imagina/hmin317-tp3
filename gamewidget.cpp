#include "gamewidget.h"

#include <QMatrix4x4>
#include <QtMath>

#include "camera.h"
#include "geometry.h"
#include "particleeffect.h"
#include "renderer.h"


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


GameWidget::GameWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    m_geometry(nullptr),
    m_terrainAABB(),
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

void GameWidget::setGeometry(Geometry *geom)
{
    if (m_geometry != geom) {
        m_geometry = geom;
    }
}

void GameWidget::setParticleEffect(ParticleEffect *effect)
{
    if (m_particleEffect != effect) {
        m_particleEffect = effect;
    }
}

void GameWidget::setRendererDirty()
{
    m_renderer->setDirty();

    m_terrainAABB.processVertices(m_geometry->vertices);
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

Season GameWidget::season() const
{
    return m_currentSeason;
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
    //TODO pass dt to Renderer

    if (m_renderer->isDirty()) {
        m_renderer->updateBuffers(m_geometry, m_particleEffect);
        m_renderer->unsetDirty();
    }

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

    const std::pair<float, float> yBounds = m_terrainAABB.yBounds();
    const QColor drawColor = colorFromSeason(m_currentSeason);

    //TODO helper classes for uniforms
    QVariantMap uniforms {
        {"viewMatrix", viewMatrix},
        {"projectionMatrix", projectionMatrix},
        {"worldMatrix", worldMatrix},
        {"minHeight", yBounds.first},
        {"maxHeight", yBounds.second},
        {"terrainColor", drawColor},
        {"particleColor", drawColor},
        {"particlesSize", m_particleEffect->particlesSize()}
    };

    m_renderer->updateUniforms(uniforms);
    m_renderer->render();
}
