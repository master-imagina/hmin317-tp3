#include "cameracontroller.h"

#include "input/frameaction.h"
#include "input/keyboard.h"

#include "render/camera.h"


namespace {

QVector3D computeDirectionFromKeys(Keyboard &keyboard, Camera &camera)
{
    const QVector3D viewVec = camera.viewVector().normalized();
    const QVector3D rightVec = camera.rightVector();

    QVector3D ret;

    if (keyboard.keyIsPressed(Qt::Key_Z)) {
        ret = viewVec;
    }
    if (keyboard.keyIsPressed(Qt::Key_S)) {
        ret = viewVec * -1;
    }
    if (keyboard.keyIsPressed(Qt::Key_Q)) {
        ret = rightVec * -1;
    }
    if (keyboard.keyIsPressed(Qt::Key_D)) {
        ret = rightVec;
    }

    return ret;
}

void updateCamera(entityx::Entity &entity, float dt)
{
    Camera &camera = *entity.component<Camera>().get();
    Keyboard &keyboard = *entity.component<Keyboard>().get();

    const QVector3D oldEye = camera.eyePos();
    const QVector3D oldTarget = camera.targetPos();

    const float speed = (keyboard.keyIsPressed(Qt::Key_Shift)) ? 100.f * 4.f : 100.f;
    const float dtFactor = (dt / 1000.0f);
    const float moveAmount = dtFactor * speed;

    const QVector3D moveDirection = computeDirectionFromKeys(keyboard, camera);

    const QVector3D newEye = moveDirection * moveAmount + oldEye;
    const QVector3D newTarget = moveDirection * moveAmount + oldTarget;

    camera.setEyePos(newEye);
    camera.setTargetPos(newTarget);
}

} // anon amespace


void createCameraController(entityx::Entity &entity)
{
    entity.assign<Camera>();
    entity.assign<Keyboard>();
    entity.assign<FrameAction>(&updateCamera);
}
