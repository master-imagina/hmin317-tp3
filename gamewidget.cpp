#include "gamewidget.h"

#include "camera.h"
#include "scene.h"

#include "renderer/renderer.h"


GameWidget::GameWidget(Scene *scene, QWidget *parent) :
    QOpenGLWidget(parent),
    m_scene(scene),
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
    m_renderer->initialize(m_scene);
}

void GameWidget::resizeGL(int w, int h)
{
    m_camera->setAspectRatio(qreal(w) / qreal(h ? h : 1.f));
}

void GameWidget::paintGL()
{
    m_renderer->render(m_scene, m_deltaTime);
}
