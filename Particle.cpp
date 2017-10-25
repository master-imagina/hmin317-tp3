#include "Particle.hpp"
#include "ParticleEmitter.hpp"

Particle::Particle(const ModelParticle &model, QVector3D position):
    m_alive(true), m_actualLife(0), m_pos(position), m_model(model)
{
    m_actualLife = m_model.getLife();
    m_plan.initPlaneGeometry();
}

Particle::Particle(const Particle& p): m_alive(p.m_alive), m_actualLife(p.m_actualLife),
                                       m_pos(p.m_pos), m_model(p.m_model)
{
    m_plan.initPlaneGeometry();
}

Particle::~Particle()
{}

Particle &Particle::operator=(const Particle& p)
{
    m_alive = p.m_alive;
    m_actualLife = p.m_actualLife;
    m_pos = p.m_pos;
    m_model = p.m_model;
    m_plan.initPlaneGeometry();
}

bool Particle::isAlive() const
{
    return m_alive;
}

void Particle::setActualLife(float actualLife)
{
    m_actualLife = actualLife;
    m_alive = m_actualLife > 0;
}

float Particle::getActualLife() const
{
    return m_actualLife;
}

void Particle::setActualZigZag(const QVector3D &actualZigZag)
{
    m_actualZigZag = actualZigZag;
}

const QVector3D &Particle::getActualZigZag() const
{
    return m_actualZigZag;
}

void Particle::setPosition(const QVector3D &pos)
{
    m_pos = pos;
}

const QVector3D &Particle::getPosition() const
{
    return m_pos;
}

void Particle::setModel(const ModelParticle &model)
{
    m_model = model;
}

const ModelParticle &Particle::getModel() const
{
    return m_model;
}

void Particle::update(const ParticleEmitter& emit, float delta)
{
    if(m_alive && delta > 0.f)
    {
        delta /= 1000.f;
        m_actualLife = m_actualLife < delta ? 0 : m_actualLife - delta;

        if(m_pos.y() > (m_model.getGround() + emit.getPostion().y()))
            m_pos.setY(m_pos.y() - (ModelParticle::MASS * m_model.getMass() * delta));
        else
            m_pos.setY(m_model.getGround() + emit.getPostion().y());

        m_alive = m_actualLife > 0;
    }
}

void Particle::draw(QMatrix4x4 &proj, QOpenGLShaderProgram *program)
{
    if(m_alive)
    {
        // Enable depth buffer
        glDisable(GL_DEPTH_TEST);

        // Enable back face culling
        glDisable(GL_CULL_FACE);

        // Transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_model.getTexture()->bind();

        QMatrix4x4 matrix;
        matrix.translate(m_pos);
        matrix.scale(m_model.getSize());
        QQuaternion framing = QQuaternion::fromAxisAndAngle(QVector3D(1,0,0),-70.0);
        //matrix.rotate(framing);
       // matrix.translate(0.0, -1.8, 0.0);



        // Set modelview-projection matrix
        program->setUniformValue("mvp_matrix", proj * matrix);

        // Use texture unit 0 which contains cube.png
        program->setUniformValue("texture", 0);

        // Draw cube geometry
        m_plan.drawPlaneGeometry(program);

        // Enable depth buffer
        glEnable(GL_DEPTH_TEST);

        // Enable back face culling
        glEnable(GL_CULL_FACE);

        // Transparency
        glDisable(GL_BLEND);
    }
}

void Particle::resetLife()
{
    float random = static_cast<float>(rand() % 200) / 50.f;
    setActualLife(m_model.getLife() * (random));
}
