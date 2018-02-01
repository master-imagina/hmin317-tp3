#ifndef ICOMPONENTUIHANDLER_H
#define ICOMPONENTUIHANDLER_H

#include <QString>

#include <entityx/Entity.h>

class EntityListModel;

class QAction;
class QVBoxLayout;
class QWidget;


class IComponentUiHandler
{
public:
    IComponentUiHandler() = default;
    ~IComponentUiHandler() = default;

    virtual QString componentName() = 0;

    virtual bool hasComponent(entityx::Entity entity) = 0;

    virtual void configureAddAction(entityx::Entity &entity,
                                    QAction *action) = 0;
    virtual void configureRemoveAction(entityx::Entity &entity,
                                       QAction *action) = 0;

    virtual QWidget *createComponentEditor(entityx::Entity entity,
                                           EntityListModel *entityListModel,
                                           QWidget *parent,
                                           const QString &projectPath) = 0;
};


template <class Component>
class TComponentUiHandler : public IComponentUiHandler
{
public:
    using ComponentType = Component;

    bool hasComponent(entityx::Entity entity) override final
    {
        return entity.has_component<Component>();
    }
};

#endif // ICOMPONENTUIHANDLER_H
