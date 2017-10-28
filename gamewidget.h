#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <memory>

#include <QOpenGLWidget>
#include <QQuaternion>
#include <QVector2D>

#include "aabb.h"

class AABoundingBox;
class Camera;
class Geometry;
class ParticleEffect;
class Renderer;
class Scene;


enum class Season
{
    None,
    Autumn,
    Winter,
    Spring,
    Summer
};


class GameWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit GameWidget(Scene *scene, QWidget *parent = 0);
    ~GameWidget();

    Camera *camera() const;
    void setCamera(Camera *camera);

    void startNewFrame(float dt);

public Q_SLOTS:
    void setSeason(Season season);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    Scene *m_scene;
    std::unique_ptr<Renderer> m_renderer;

    Camera *m_camera;

    float m_deltaTime;
    Season m_currentSeason;
};

#endif // GAMEWIDGET_H
