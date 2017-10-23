#ifndef CAMERA_FACING_H
#define CAMERA_FACING_H

#include "../gameobject/component.h"

class CameraFacingController : public Component {

public:

    // Basic constructor
    CameraFacingController(GameObject * parent);

    // Update callback
    virtual void update() override;

    // Component override
    virtual void destroy() override;

    // Component override
    virtual int component() const override;

    // Component override
    virtual void clone(GameObject * c) override;

    // Indicate if the componant is an instance of CameraFacingController
    static bool isInstance(Component * c);

};

#endif // CAMERA_FACING_H
