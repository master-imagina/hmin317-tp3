#include "graphicmanager.h"

float GraphicManager::window_width = 1;
float GraphicManager::window_height = 1;

float absf(float nb){
    if(nb >= 0)
        return nb;
    return -nb;
}

GraphicManager::GraphicManager(QWidget *parent) :
    QOpenGLWidget(parent),
    texture(0),
    texture_snow(0),
    texture_rain(0)
{
    angle = 0;
    p_x = 0;
    p_y = 0;
}

GraphicManager::~GraphicManager(){
    makeCurrent();
    delete texture;
    delete texture_snow;
    delete texture_rain;
    delete terrain;
    delete cameraV;
    delete cameraFF;
    doneCurrent();
}

void GraphicManager::wheelEvent(QWheelEvent *event){

    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    InputManager::setWheel(numSteps,event->orientation() == Qt::Horizontal);

    update();
}

void GraphicManager::mousePressEvent(QMouseEvent *e){

    InputManager::setButtonDown(e->button());

    mousePressPosition = QVector2D(e->localPos());

    update();
}

void GraphicManager::mouseMoveEvent(QMouseEvent *e){

    QVector2D diff = 0.5*(QVector2D(e->localPos()) - mousePressPosition);
    mousePressPosition = QVector2D(e->localPos());

    if(absf(diff.x()) > 0.05)
        p_x = diff.x();
    if(absf(diff.y()) > 0.05)
        p_y = diff.y();

    update();
}

void GraphicManager::mouseReleaseEvent(QMouseEvent *e){

    InputManager::setButtonUp(e->button());

    update();
}

void GraphicManager::timerEvent(QTimerEvent *) {

}

void GraphicManager::initializeGL(){

    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Use QBasicTimer because its faster than QTimer
    timer.start();

    Draw::Initialization(&program);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //vbo_plane = Draw_3D::Init_Plane(32,32,1.0);

    cameraV = new CameraView(0,0,15,0,0,0);
    //cameraFF = new CameraFreeFly(0,0,15,0,0,0);
    //cameraFF->useVerticalUp(false);

    terrain = new Terrain("heightmap-3.png",10,10,1);
    vbo_plane = Draw_3D::Init_Plane("heightmap-3.png");
    saison = -1;
    timeBeforeChangeSaison = -1;
    debug = false;

    emitter = new ParticleSystem();
    emitter->duration = 5000;
    emitter->emissionSphere = QVector3D(10,0,10);
    emitter->gravity = 1;
    emitter->lifetime_ms = 10000;
    emitter->loop = true;
    emitter->nb_particles = 100;
    emitter->position = QVector3D(0,20,0);

}

void GraphicManager::initShaders()
{
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();
    if (!program.link())
        close();
    if (!program.bind())
        close();
}

void GraphicManager::initTextures()
{
    texture = new QOpenGLTexture(QImage(":/heightmap-3.png").mirrored());
        texture->setMinificationFilter(QOpenGLTexture::Nearest);
        texture->setMagnificationFilter(QOpenGLTexture::Linear);
        texture->setWrapMode(QOpenGLTexture::Repeat);

    texture_snow = new QOpenGLTexture(QImage(":/snow.png").mirrored());
        texture_snow->setMinificationFilter(QOpenGLTexture::Nearest);
        texture_snow->setMagnificationFilter(QOpenGLTexture::Linear);
        texture_snow->setWrapMode(QOpenGLTexture::Repeat);

    texture_rain = new QOpenGLTexture(QImage(":/rain.png").mirrored());
        texture_rain->setMinificationFilter(QOpenGLTexture::Nearest);
        texture_rain->setMagnificationFilter(QOpenGLTexture::Linear);
        texture_rain->setWrapMode(QOpenGLTexture::Repeat);
}

void GraphicManager::resizeGL(int w, int h)
{
    window_width = w;
    window_height = h;
}

void GraphicManager::keyPressEvent(QKeyEvent *e){
    InputManager::setKeyDown(e->key());
    update();
}

void GraphicManager::keyReleaseEvent(QKeyEvent *e){
    InputManager::setKeyUp(e->key());
    update();
}

int GraphicManager::getElapsedTime(){
    int timer_elapsed = timer.elapsed() - timer_prec;
    timer_prec = timer.elapsed();
    return timer_elapsed;
}

void GraphicManager::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(debug)
        cout << "Bind Texture" << endl;

    //texture->bind();
    //texture_snow->bind();

    if(debug)
        cout << "Start3D" << endl;
    Draw::start3D();
    Draw::enableTexture(true);

    int timer_elapsed = getElapsedTime();

    if(timeBeforeChangeSaison == -1 || timeBeforeChangeSaison >= 3000){
        timeBeforeChangeSaison = 0;
        updateSaison();
    } else {
        timeBeforeChangeSaison += timer_elapsed;
    }

    Draw::setColor(QVector4D(1,1,1,1));
    switch (saison) {
    case 1:
        emitter->gravity = 10;
        emitter->emission = true;
        break;
    case 2:
        emitter->emission = false;
        break;
    case 3:
        emitter->gravity = 10;
        emitter->emission = true;
        break;
    default:
        emitter->gravity = 1;
        emitter->emission = true;
        break;
    }

    if(debug)
        cout << "Draw Camera" << endl;
        InputManager::Update(timer_elapsed);//*
        cameraV->Update(-InputManager::getVerticalWheel() + InputManager::getVariationBetweenKeys_Smooth(Qt::Key_A,Qt::Key_E)*(-2),
                       p_x + InputManager::getVariationBetweenKeys_Smooth(Qt::Key_Q,Qt::Key_D),
                       p_y + InputManager::getVariationBetweenKeys_Smooth(Qt::Key_Z,Qt::Key_S),
                       10.0,
                       20.0,
                       timer_elapsed);//*/
        /*
        cameraFF->Update(QVector3D(InputManager::getVariationBetweenKeys_Smooth(Qt::Key_Z,Qt::Key_S),
                                   InputManager::getVariationBetweenKeys_Smooth(Qt::Key_A,Qt::Key_E),
                                   InputManager::getVariationBetweenKeys_Smooth(Qt::Key_D,Qt::Key_Q)),
                         p_x,
                         -p_y,
                         1.0,
                         10.0,
                         timer_elapsed );//*/

        p_x *= 0.0;
        p_y *= 0.0;

        if(debug)
            cout << "Draw 3D Objects" << endl;

        program.setUniformValue("texture", 0);

        if(saison == 0)
            texture_snow->bind();
        else
            texture_rain->bind();

        emitter->cameraDirection = cameraV->getDirection();
        emitter->cameraPosition = Camera::position;

        emitter->Run(timer_elapsed);

        switch (saison) {
        case 1:
            Draw::setColor(QVector4D(0.5,0,0.5,1));
            break;
        case 2:
            Draw::setColor(QVector4D(0.5,1,0.1,1));
            break;
        case 3:
            Draw::setColor(QVector4D(1,0.5,0,1));
            break;
        default:
            Draw::setColor(QVector4D(1,1,1,1));
            break;
        }

        texture->bind();
        terrain->Draw();


    if(debug)
        cout << "Start2D" << endl;
    Draw::start2D(0,window_width,0,window_height,0.1,1000);

    if(debug)
        cout << "Draw 2D Objects" << endl;
        Draw_2D::Square(QVector2D(0,0),QVector2D(200,200));
        Draw::enableTexture(false);
        Draw::setColor(QVector4D(1,1,0,1));
        Draw::translate(100-Camera::position.x()*200.0/terrain->size_x,100+Camera::position.z()*200.0/terrain->size_z,0);
        Draw_2D::Square(QVector2D(0,0),QVector2D(10,10));
    update();
}

void GraphicManager::changeSaison(int s){
    saison = s;
}
