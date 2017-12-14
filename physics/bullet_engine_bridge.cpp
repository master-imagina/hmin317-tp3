#include "bullet_engine_bridge.h"

#include "render/transform.h"


btTransform transformToBtTransform(const Transform &transform)
{
    const QVector3D pos = transform.translate();
    const QVector3D rotation = transform.rotation();

    // Below y() and x() are swapped, due to the btQuaternion prototype
    const btQuaternion quat(rotation.y(), rotation.x(), rotation.z());

    return btTransform(quat, btVector3(pos.x(), pos.y(), pos.z()));
}
