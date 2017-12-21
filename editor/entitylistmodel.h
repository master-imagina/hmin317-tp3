#ifndef ENTITYLISTMODEL_H
#define ENTITYLISTMODEL_H

#include <QComboBox>
#include <QStandardItemModel>

#include <entityx/Entity.h>

#include "core/scene.h"


class EntityListModel : public QStandardItemModel
{
public:
    explicit EntityListModel(Scene &scene, QObject *parent = nullptr);

    QModelIndex indexFromEntityId(const entityx::Entity::Id &entity);

private:
    void onEntityAdded(const entityx::Entity::Id &entity);
    void onEntityRemoved(const entityx::Entity::Id &entity);
    void onSceneCleared();

private:
    Scene &m_theScene;

    std::unordered_map<entityx::Entity::Id, QStandardItem *> m_entityToItem;
};

#endif // ENTITYLISTMODEL_H
