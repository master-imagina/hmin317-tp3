#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <memory>

#include <QOpenGLWidget>

#include "core/aliases_memory.h"

class Camera;
class Renderer;


class RenderWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit RenderWidget(QWidget *parent = 0);
    ~RenderWidget();

    Camera *camera() const;
    void setCamera(Camera *camera);

    Renderer *renderer() const;

    void startNewFrame(float dt);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

protected:
    Camera *m_camera;
    uptr<Renderer> m_renderer;

    float m_deltaTime;
};

#endif // RENDERWIDGET_H
