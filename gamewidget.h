#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <memory>

#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QVector2D>

#include "aabb.h"

class AABoundingBox;
class Camera;
class Geometry;
class Renderer;


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
    explicit GameWidget(QWidget *parent = 0);
    ~GameWidget();

    void setGeometry(Geometry *geom);
    void setRendererDirty();

    Camera *camera() const;
    void setCamera(Camera *camera);

    Season season() const;

    void startNewFrame(float dt);

public Q_SLOTS:
    void setSeason(Season season);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void initShaders();

private:
    QOpenGLShaderProgram m_shaderProgram;

    Geometry *m_geometry;
    AABoundingBox m_terrainAABB;
    std::unique_ptr<Renderer> m_renderer;

    Camera *m_camera;

    float m_deltaTime;
    Season m_currentSeason;
};

#endif // GAMEWIDGET_H
