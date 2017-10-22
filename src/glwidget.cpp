#include "glwidget.h"
#include "global.h"
#include "gameobject/geometry.h"
#include "gameobject/rigidbody.h"
#include "gameobject/material.h"
#include "gameobject/camera.h"
#include "gameobject/particle_system.h"
#include "controller/camera_rts.h"
#include "controller/camera_facing.h"
#include "geometry/shapes.h"

#include <iostream>

const int GLWidget::DefaultFrameRate = 60;
const char * GLWidget::DefaultVShader = "./shaders/vshader.glsl";
const char * GLWidget::DefaultFShader = "./shaders/fshader.glsl";

int GLWidget::WindowCount = 0;

GLWidget::GLWidget(QWidget *parent, int season) :
    QOpenGLWidget(parent),
    camera(new GameObject()),
    terrain(new GameObject()),
    snow(new GameObject()),
    rain(new GameObject()),
    season(season)
{
    WindowCount++;

    // On lance le timer d'update
    updateTimer.start(1000 / GLWidget::DefaultFrameRate, this);

    // Active la récupération des événements clavier
    grabKeyboard();

    camera->transform().position.setX(1.0f);
    camera->transform().position.setY(1.0f);
    camera->addComponent<Camera>();
    camera->addComponent<CameraRTSController>();
}

GLWidget::~GLWidget() {
    // Définie le contexte courant pour détruire les textures et buffers
    makeCurrent();
    // Detruit les objets OpenGL
    texture.reset();
    textureTerrain.reset();
    textureSnow.reset();
    textureRain.reset();
    delete camera;
    delete terrain;
    delete snow;
    delete rain;
    if (--WindowCount == 0)
        Material::defaultTexture().reset();
    doneCurrent();
}



void GLWidget::initShaders() {
    // Compile le vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, GLWidget::DefaultVShader))
        close();

    // Compile le fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, GLWidget::DefaultFShader))
        close();

    // Lie pipeline aux shaders
    if (!program.link())
        close();

    // Attache le pipeline pour être utilisé
    if (!program.bind())
        close();
}

void GLWidget::initTexture() {
    makeCurrent();
    texture = Material::defaultTexture();

    textureTerrain = std::make_shared<QOpenGLTexture>(QImage("./res/heightmap-3.png").mirrored());
    textureTerrain->setMinificationFilter(QOpenGLTexture::Nearest);
    textureTerrain->setMagnificationFilter(QOpenGLTexture::Linear);
    textureTerrain->setWrapMode(QOpenGLTexture::Repeat);
    textureSnow = std::make_shared<QOpenGLTexture>(QImage("./res/flocon.png").mirrored());
    textureSnow->setMinificationFilter(QOpenGLTexture::Nearest);
    textureSnow->setMagnificationFilter(QOpenGLTexture::Linear);
    textureSnow->setWrapMode(QOpenGLTexture::Repeat);
    textureRain = std::make_shared<QOpenGLTexture>(QImage("./res/goutte.png").mirrored());
    textureRain->setMinificationFilter(QOpenGLTexture::Nearest);
    textureRain->setMagnificationFilter(QOpenGLTexture::Linear);
    textureRain->setWrapMode(QOpenGLTexture::Repeat);

    // TERRAIN
    QImage heightmap("./res/heightmap-3.png");
    GeometryTerrain(terrain->addComponent<Geometry>(), heightmap);
    terrain->addComponent<Material>()->texture = textureTerrain;

    // SNOW
    GameObject * particle = new GameObject();
    particle->transform().position.setY(2.0f);
    particle->transform().scale = QVector3D(0.05f, 0.05f, 1.0f);
    GeometryUIPlane(particle->addComponent<Geometry>());
    particle->addComponent<Rigidbody>();
    particle->getComponent<Rigidbody>()->gravity.setY(-0.05f);
    particle->addComponent<Material>();
    particle->getComponent<Material>()->texture = textureSnow;
    particle->addComponent<CameraFacingController>();
    ParticleSystem * ps = snow->addComponent<ParticleSystem>();
    Mesh tmp_m;
    tmp_m.addVertex(QVector3D(-1.0f, 0.0f, -1.0f), QVector2D(0.0f, 0.0f));
    tmp_m.addVertex(QVector3D( 1.0f, 0.0f, -1.0f), QVector2D(1.0f, 0.0f));
    tmp_m.addVertex(QVector3D(-1.0f, 0.0f,  1.0f), QVector2D(0.0f, 1.0f));
    tmp_m.addVertex(QVector3D( 1.0f, 0.0f,  1.0f), QVector2D(1.0f, 1.0f));
    tmp_m.addTriangle(0, 1, 2);
    tmp_m.addTriangle(1, 3, 2);
    tmp_m.addVertex(QVector3D(), QVector2D());
    ps->setEmitter(&tmp_m);
    ps->setParticle(particle);
    ps->ParticleDuration = 2800;
    ps->ParticleFrequency = 20;

    // RAIN
    particle = new GameObject();
    particle->transform().position.setY(2.0f);
    particle->transform().scale = QVector3D(0.01f, 0.05f, 1.0f);
    GeometryUIPlane(particle->addComponent<Geometry>());
    particle->addComponent<Rigidbody>();
    particle->addComponent<Material>();
    particle->getComponent<Material>()->texture = textureRain;
    particle->addComponent<CameraFacingController>();
    ps = rain->addComponent<ParticleSystem>();
    ps->setEmitter(&tmp_m);
    ps->setParticle(particle);
    doneCurrent();
}



void GLWidget::timerEvent(QTimerEvent *e) {
    makeCurrent();
    camera->update();
    rain->update();
    snow->update();

    e;

    // Met à jour le rendu
    update();
    doneCurrent();

    updateSaison();
}

void GLWidget::keyPressEvent(QKeyEvent * event) {
    camera->keyPressEvent(event);
}

void GLWidget::keyReleaseEvent(QKeyEvent * event) {
    camera->keyReleaseEvent(event);
}

void GLWidget::mousePressEvent(QMouseEvent * event) {
    camera->mousePressEvent(event);
}

void GLWidget::mouseReleaseEvent(QMouseEvent * event) {
    camera->mouseReleaseEvent(event);
}

void GLWidget::mouseMoveEvent(QMouseEvent * event) {
    camera->mouseMoveEvent(event);
}

void GLWidget::wheelEvent(QWheelEvent * event) {
    camera->wheelEvent(event);
}

void GLWidget::initializeGL() {
    // Initialise les fonctions OpenGL
    initializeOpenGLFunctions();

    // Change la couleur par défaut
    glClearColor(0, 0, 0, 1);

    // Active le buffer de profondeur
    glEnable(GL_DEPTH_TEST);

    // Active le backface culling
    glEnable(GL_CULL_FACE);

    initShaders();
    initTexture();
}

void GLWidget::resizeGL(int w, int h) {
    camera->getComponent<Camera>()->aspect = (float)w / (float)(h ? h : 1);
}

void GLWidget::paintGL() {
    // Réinitialise les buffer de couleur et de profondeur
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Attache le pipeline pour être utilisé
    if (!program.bind())
        close();

    // Attache la texture pour être utilisée
    //texture->bind();

    // Assign projection and view matrix
    camera->getComponent<Camera>()->apply(&program);

    // Assigne la texture dans le fragment shader
    program.setUniformValue("texture", 0);

    // Assigne la position de la lumière et  sa couleur
    program.setUniformValue("v_lightpos", QVector3D(0.0f, 2.0f, -1.0f));
    if (season == 0)
        program.setUniformValue("v_lightcolor", QVector3D(0.5f, 1.0f, 0.0f));
    if (season == 1)
        program.setUniformValue("v_lightcolor", QVector3D(1.0f, 1.0f, 0.0f));
    if (season == 2)
        program.setUniformValue("v_lightcolor", QVector3D(1.0f, 0.5f, 0.0f));
    if (season == 3)
        program.setUniformValue("v_lightcolor", QVector3D(1.0f, 1.0f, 1.0f));

    terrain->paintGL(&program);
    if (season == 2)
        rain->paintGL(&program);
    if (season == 3)
        snow->paintGL(&program);
}

void GLWidget::changeSaison(int s) {
    season = s;
}
