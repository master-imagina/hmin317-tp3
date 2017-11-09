#include "renderwidget.h"

#include "3rdparty/entityx/System.h"

#include "render/camera.h"
#include "render/rendersystem.h"

#include "render/renderer/renderer.h"


RenderWidget::RenderWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    m_camera(nullptr),
    m_renderer(std::make_unique<Renderer>())
{}

RenderWidget::~RenderWidget()
{
    // Make sure the context is current before freeing allocated resources
    makeCurrent();

    m_renderer->cleanup();

    doneCurrent();
}

Camera *RenderWidget::camera() const
{
    return m_camera;
}

void RenderWidget::setCamera(Camera *camera)
{
    if (m_camera != camera) {
        m_camera = camera;
    }
}

Renderer *RenderWidget::renderer() const
{
    return m_renderer.get();
}

void RenderWidget::startNewFrame(float dt)
{
    m_deltaTime = dt;

    update();
}

void RenderWidget::initializeGL()
{
    m_renderer->initialize();
}

void RenderWidget::resizeGL(int w, int h)
{
    m_camera->setAspectRatio(qreal(w) / qreal(h ? h : 1.f));
}

void RenderWidget::paintGL()
{
    m_renderer->render(*m_camera, m_deltaTime);
}
