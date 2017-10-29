#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <memory>

#include <QOpenGLWidget>
#include <QQuaternion>
#include <QVector2D>

class Camera;
class Renderer;
class SeasonController;
class Scene;


class GameWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit GameWidget(Scene *scene, SeasonController *seasons, QWidget *parent = 0);
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
    SeasonController *m_seasons;
    std::unique_ptr<Renderer> m_renderer;

    Camera *m_camera;

    float m_deltaTime;
};

#endif // GAMEWIDGET_H
