#include "sceneview.h"

#include <QToolBar>
#include <QToolButton>
#include <QTreeWidget>

#include "core/scene.h"

#include "render/transform.h"


SceneView::SceneView(Scene &scene, QWidget *parent) :
    ScrollablePane(tr("Scene Outline"), parent),
    m_scene(scene),
    m_entityTreeView(nullptr),
    m_itemToEntity()
{
    m_entityTreeView = new QTreeWidget(this);
    m_entityTreeView->setHeaderHidden(true);

    setWidget(m_entityTreeView);

    createToolbar();
    createConnections();
}

void SceneView::createToolbar()
{
    auto *toolbar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, toolbar);

    auto *addBtn = new QToolButton(toolbar);
    addBtn->setText(tr("Add"));

    auto *removeBtn = new QToolButton(toolbar);
    removeBtn->setText(tr("Remove"));

    toolbar->addWidget(addBtn);
    toolbar->addWidget(removeBtn);

    connect(addBtn, &QToolButton::clicked,
            &m_scene, &Scene::createEntity);

    connect(removeBtn, &QToolButton::clicked,
            [this] {
        entityx::Entity entityToRemove;

        QTreeWidgetItem *currentEntityItem = m_entityTreeView->currentItem();

        if (currentEntityItem) {
            entityToRemove = m_itemToEntity.at(currentEntityItem);

            m_scene.removeEntity(entityToRemove);
        }
    });
}

void SceneView::createConnections()
{
    connect(&m_scene, &Scene::entityAdded,
            this, &SceneView::onEntityAdded);

    connect(&m_scene, &Scene::entityRemoved,
            this, &SceneView::onEntityRemoved);

    connect(&m_scene, &Scene::cleared,
            this, &SceneView::onSceneCleared);

    connect(m_entityTreeView, &QTreeWidget::currentItemChanged,
            [this] (QTreeWidgetItem *current, QTreeWidgetItem *previous) {
        Q_UNUSED (previous);

        entityx::Entity entity;

        if (current) {
            entity = m_itemToEntity.at(current);
        }

        Q_EMIT entityItemSelected(entity);
    });
}

void SceneView::onEntityAdded(entityx::Entity entity)
{
    entity.assign<Transform>();

    auto *item = new QTreeWidgetItem;
    item->setText(0, QString::number(entity.id().index()));

    m_itemToEntity.emplace(item, entity);

    m_entityTreeView->addTopLevelItem(item);
    m_entityTreeView->setCurrentItem(item);
}

void SceneView::onEntityRemoved(entityx::Entity entity)
{
    auto itemIt = std::find_if(m_itemToEntity.begin(), m_itemToEntity.end(),
                               [entity] (const auto &itemAndEntity) {
        return entity == itemAndEntity.second;
    });

    delete m_entityTreeView->takeTopLevelItem(m_entityTreeView->indexOfTopLevelItem(itemIt->first));

    m_itemToEntity.erase(itemIt);
}

void SceneView::onSceneCleared()
{
    m_entityTreeView->clear();
    m_itemToEntity.clear();
}
