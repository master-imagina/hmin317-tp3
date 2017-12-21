#ifndef CAMERACONTROLS_H
#define CAMERACONTROLS_H

#include <QFrame>

#include <entityx/Entity.h>

class Camera;
class GameWidget;
class Scene;

class QComboBox;


class CameraControls : public QFrame
{
    Q_OBJECT

public:
    explicit CameraControls(Camera *camera,
                            Scene &scene,
                            GameWidget *gameWidget);

    Camera *getCamera() const;
    void setCamera(Camera *camera);

Q_SIGNALS:
    void cameraChanged(Camera *camera);

public Q_SLOTS:
    void onEntityComponentAdded(entityx::Entity entity, const QString &compName);
    void onEntityComponentRemoved(entityx::Entity entity, const QString &compName);

private:
    Camera *m_camera;

    QComboBox *m_currentCameraComboBox;

    GameWidget *m_theGameWidget;

    std::vector<entityx::Entity> m_entitiesWithCamera;

    float m_lookupSpeed;            // Speed when using the lookup button
    float m_linearSpeed;            // Speed when using all other buttons
};

#endif // CAMERACONTROLS_H
