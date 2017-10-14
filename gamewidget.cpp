#include "gamewidget.h"

#include <QMatrix4x4>
#include <QPainter>
#include <QtMath>

#include "camera.h"
#include "renderer.h"
#include "geometry.h"


GameWidget::GameWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    m_shaderProgram(),
    m_geometry(nullptr),
    m_terrainAABB(),
    m_renderer(nullptr),
    m_camera(nullptr),
    m_currentSeason(Season::None)
{}

GameWidget::~GameWidget()
{
    // Make sure the context is current before freeing allocated resources
    makeCurrent();

    m_renderer->cleanupResources();

    doneCurrent();
}

void GameWidget::setGeometry(Geometry *geom)
{
    if (m_geometry != geom) {
        m_geometry = geom;
    }
}

void GameWidget::setRendererDirty()
{
    m_renderer->updateBuffers(m_geometry);

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

void GameWidget::startNewFrame(float dt)
{
    m_deltaTime = dt;

    update();
}

void GameWidget::setSeason(Season season)
{
    if (m_currentSeason != season) {
        m_currentSeason = season;
    }
}

void GameWidget::initializeGL()
{
    m_renderer = std::make_unique<Renderer>();
    m_renderer->initialize();

    initShaders();
}

void GameWidget::initShaders()
{
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                 "://res/shaders/geom_textured.vert")) {
        close();
    }

    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                 ":/res/shaders/terrain_heightmap.frag")) {
        close();
    }

    if (!m_shaderProgram.link()) {
        close();
    }

    if (!m_shaderProgram.bind()) {
        close();
    }
}

void GameWidget::resizeGL(int w, int h)
{
    m_camera->setAspectRatio(qreal(w) / qreal(h ? h : 1.f));
}

void GameWidget::paintGL()
{
    // Send uniforms to shaders
    const QMatrix4x4 viewMatrix = m_camera->viewMatrix();
    const QMatrix4x4 projectionMatrix = m_camera->projectionMatrix();

    m_shaderProgram.setUniformValue("mvp_matrix",
                                    projectionMatrix * viewMatrix);

    const std::pair<float, float> yBounds = m_terrainAABB.yBounds();
    m_shaderProgram.setUniformValue("minHeight", yBounds.first);
    m_shaderProgram.setUniformValue("maxHeight", yBounds.second);

    // Draw geometry
    m_renderer->clearForNewFrame();
    m_renderer->draw(m_geometry, &m_shaderProgram);
}
