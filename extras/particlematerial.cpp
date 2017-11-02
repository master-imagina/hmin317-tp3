#include "particlematerial.h"

#include "extras/shaderutils.h"

#include "render/material/renderpass.h"


ParticleMaterial::ParticleMaterial() :
    m_viewMatrixParam(nullptr),
    m_projMatrixParam(nullptr),
    m_colorParam(nullptr),
    m_sizeParam(nullptr)
{
    uptr<ShaderProgram> particleShader = shaderProgramFromFile(
        "://res/shaders/particle.vert",
        "://res/shaders/particle.geom",
        "://res/shaders/particle.frag"
    );

    RenderPass *basePass = addRenderPass("base");
    basePass->setShaderProgram(std::move(particleShader));

    m_viewMatrixParam = basePass->addParam("viewMatrix", QMatrix4x4());
    m_projMatrixParam = basePass->addParam("projectionMatrix", QMatrix4x4());
    m_colorParam = basePass->addParam("particleColor", QColor());
    m_sizeParam = basePass->addParam("particlesSize", 4.f);
}

QMatrix4x4 ParticleMaterial::viewMatrix() const
{
    return m_viewMatrixParam->value.value<QMatrix4x4>();
}

void ParticleMaterial::setViewMatrix(const QMatrix4x4 &mat)
{
    m_viewMatrixParam->value = mat;
}

QMatrix4x4 ParticleMaterial::projectionMatrix() const
{
    return m_projMatrixParam->value.value<QMatrix4x4>();
}

void ParticleMaterial::setProjectionMatrix(const QMatrix4x4 &mat)
{
    m_projMatrixParam->value = mat;
}

QColor ParticleMaterial::color() const
{
    return m_colorParam->value.value<QColor>();
}

void ParticleMaterial::setColor(const QColor &color)
{
    m_colorParam->value = color;
}

float ParticleMaterial::size() const
{
    return m_sizeParam->value.toFloat();
}

void ParticleMaterial::setSize(float size)
{
    m_sizeParam->value = size;
}
