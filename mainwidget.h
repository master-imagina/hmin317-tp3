#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "geometryengine.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <iostream>
#include "particleemitter.h"

class GeometryEngine;

class MainWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    void setFPS(int _fps);
    void setTimer(QTimer *);
    void setSeason(int);
    void setRGB(float _r, float _g, float _b) {r=_r;g=_g;b=_b;}

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();
    void initTextures();

private slots:
    void changeSeason(){season++;if (season == 4) season = 0;}

private:
    QBasicTimer timer;
    QOpenGLShaderProgram program;
    GeometryEngine *geometries;

    QOpenGLTexture *texture;

    QMatrix4x4 projection;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed;
    QQuaternion rotation;
    float fPositionX;
    float fPositionY;
    float fPositionZ;
    bool bKeys[10];
    int FPS = 60;
    int season=0;
    int mySeason=0;
    float r = 0;
    float g = 0;
    float b = 0;
    QTimer * stimer;
    ParticleEmitter* snow;
    ParticleEmitter* rain;
};

#endif // MAINWIDGET_H
