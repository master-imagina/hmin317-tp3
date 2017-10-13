#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class Particles {

    static const int max_particules = 10;

    static Particles * instance;

    QOpenGLTexture * texture;
    QOpenGLShaderProgram program;

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;

public:

    Particles();
    ~Particles();

    static Particles& getInstance();

    void update();
    void paint();

};