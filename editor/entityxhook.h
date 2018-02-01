#ifndef ENTITYXHOOK_H
#define ENTITYXHOOK_H

#include <QObject>

#include <entityx/System.h>

class SystemEngine;


class EntityxHook : public QObject
{
    Q_OBJECT

public:
    EntityxHook(SystemEngine &systemEngine, QObject *parent = nullptr);
    ~EntityxHook() = default;

Q_SIGNALS:
    void entityComponentAdded(entityx::Entity entity, const QString &compName);
    void entityComponentRemoved(entityx::Entity entity, const QString &compName);

private:
    SystemEngine &m_systemEngine;
};

#endif // ENTITYXHOOK_H
