#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "component.h"

#include <QVector3D>
#include <QQuaternion>

class GameObject;

class Transform : public Component {

public:

    // GameObject position
    QVector3D position;

    // GameObject rotation
    QQuaternion rotation;

    // GameObject scale
    QVector3D scale;

    // Basic constructor
    Transform(GameObject * parent);

    // Component override
    virtual void destroy() override;

    // Component override
    virtual int component() const override;

    // Component override
    virtual void clone(GameObject * c) override;

    // Indicate if the componant is an instance of Transform
    static bool isInstance(Component * c);

};

#endif // TRANSFORM_H
