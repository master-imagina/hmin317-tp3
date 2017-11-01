#ifndef PARTICLEMATERIAL_H
#define PARTICLEMATERIAL_H

#include <QColor>
#include <QMatrix4x4>

#include "render/material.h"

class ShaderParam;


class ParticleMaterial : public Material
{
public:
    ParticleMaterial();

    QMatrix4x4 viewMatrix() const;
    void setViewMatrix(const QMatrix4x4 &mat);

    QMatrix4x4 projectionMatrix() const;
    void setProjectionMatrix(const QMatrix4x4 &mat);

    QColor color() const;
    void setColor(const QColor &color);

    float size() const;
    void setSize(float size);

private:
    ShaderParam *m_viewMatrixParam;
    ShaderParam *m_projMatrixParam;
    ShaderParam *m_colorParam;
    ShaderParam *m_sizeParam;
};

#endif // PARTICLEMATERIAL_H
