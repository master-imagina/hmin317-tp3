#ifndef MATERIAL_H
#define MATERIAL_H

#include "component.h"

#include <memory>

#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>

class Material : public Component {

public:

    // Material texture
    std::shared_ptr<QOpenGLTexture> texture;

    // Basic constructor
    Material(GameObject * parent);

    // Component override
    virtual void destroy() override;

    // Component override
    virtual int component() const override;

    // Component override
    virtual void clone(GameObject * c) override;

    // Called when OpenGL rendering
    void paintGL(QOpenGLShaderProgram *program);

    // Indicate if the componant is an instance of Geometry
    static bool isInstance(Component * c);

    // Return the default texture
    static std::shared_ptr<QOpenGLTexture>& defaultTexture();

};

#endif // MATERIAL_H
