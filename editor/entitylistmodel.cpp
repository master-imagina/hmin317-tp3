#include "entitylistmodel.h"

#include <QAbstractItemView>


////////////////////// EntityListModel //////////////////////

EntityListModel::EntityListModel(Scene &scene, QObject *parent) :
    QStandardItemModel(parent),
    m_theScene(scene),
    m_entityToItem()
{
    connect(&scene, &Scene::entityAdded,
            [this] (entityx::Entity entity) {
        onEntityAdded(entity.id());
    });

    connect(&scene, &Scene::entityRemoved,
            [this] (entityx::Entity entity) {
        onEntityRemoved(entity.id());
    });

    connect(&scene, &Scene::cleared,
            this, &EntityListModel::onSceneCleared);
}

QModelIndex EntityListModel::indexFromEntityId(const entityx::Entity::Id &id)
{
    const auto itemIt = m_entityToItem.find(id);

    if (itemIt == m_entityToItem.end()) {
        return QModelIndex();
    }

    const QStandardItem *item = itemIt->second;
    Q_ASSERT (item);

    return indexFromItem(item);
}

void EntityListModel::onEntityAdded(const entityx::Entity::Id &entityId)
{
    auto item = new QStandardItem(QString::number(entityId.index()));
    item->setData(QVariant::fromValue(entityId), Qt::UserRole + 1);

    appendRow(item);

    m_entityToItem.emplace(entityId, item);
}

void EntityListModel::onEntityRemoved(const entityx::Entity::Id &entityId)
{
    const auto itemIt = m_entityToItem.find(entityId);
    const QStandardItem *item = itemIt->second;
    const QModelIndex itemIdx = indexFromItem(item);

    removeRow(itemIdx.row());

    delete item;
    m_entityToItem.erase(itemIt);
}

void EntityListModel::onSceneCleared()
{
    m_entityToItem.clear();

    clear();
}
