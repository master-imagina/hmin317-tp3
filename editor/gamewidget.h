#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <memory>

#include <QOpenGLWidget>

class Camera;
class Renderer;
class Scene;


class GameWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit GameWidget(Scene *scene, QWidget *parent = 0);
    ~GameWidget();

    Camera *camera() const;
    void setCamera(Camera *camera);

    void startNewFrame(float dt);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    Scene *m_scene;
    std::unique_ptr<Renderer> m_renderer;

    Camera *m_camera;

    float m_deltaTime;
};

#endif // GAMEWIDGET_H
