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

    toolbar->addWidget(addBtn);

    connect(addBtn, &QToolButton::clicked,
            &m_scene, &Scene::createEntity);
}

void SceneView::createConnections()
{
    connect(&m_scene, &Scene::entityAdded,
            this, &SceneView::onEntityAdded);

    connect(&m_scene, &Scene::cleared,
            this, &SceneView::onSceneCleared);

    connect(m_entityTreeView, &QTreeWidget::currentItemChanged,
            [this] (QTreeWidgetItem *current, QTreeWidgetItem *previous) {
        Q_UNUSED (previous);

        if (current) {
            entityx::Entity entity = m_itemToEntity.at(current);

            onEntityItemActivated(entity);
        }
    });
}

void SceneView::onEntityAdded(entityx::Entity entity)
{
    entity.assign<Transform>();

    auto *item = new QTreeWidgetItem;
    item->setText(0, QString::number(entity.id().index()));

    m_itemToEntity.insert({item, entity});

    m_entityTreeView->addTopLevelItem(item);
    m_entityTreeView->setCurrentItem(item);
}

void SceneView::onEntityItemActivated(entityx::Entity entity)
{
    Q_EMIT entityItemSelected(entity);
}

void SceneView::onSceneCleared()
{
    m_entityTreeView->clear();
    m_itemToEntity.clear();
}
