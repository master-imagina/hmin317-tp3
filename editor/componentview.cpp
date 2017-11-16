#include "componentview.h"

#include <QBoxLayout>
#include <QMenu>
#include <QToolButton>

#include "editor/sceneview.h"

#include "editor/gui/layoututils.h"


ComponentView::ComponentView(SceneView *sceneView, QWidget *parent) :
    Pane(tr("Components"), parent),
    m_theSceneView(sceneView),
    m_mainWidget(nullptr),
    m_mainLayout(nullptr),
    m_currentEntity(),
    m_componentEditorCreators()
{
    Q_ASSERT (m_theSceneView);

    m_mainWidget = new QWidget(this);
    setWidget(m_mainWidget);

    auto *addComponentBtn = new QToolButton(m_mainWidget);
    addComponentBtn->setText(tr("Add Component"));
    addComponentBtn->setPopupMode(QToolButton::InstantPopup);
    addComponentBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    auto addComponentMenu = new QMenu(addComponentBtn);

    addComponentBtn->setMenu(addComponentMenu);

    m_mainLayout = new QVBoxLayout(m_mainWidget);
    m_mainLayout->addWidget(addComponentBtn);
    m_mainLayout->addStretch();


    createConnections();
}

void ComponentView::addComponentEditorCreator(ComponentEditorCreator creator)
{
    m_componentEditorCreators.emplace_back(creator);
}

void ComponentView::setCurrentEntity(entityx::Entity entity)
{
    m_currentEntity = entity;

    clearLayout(m_mainLayout, 1);

    m_mainLayout->addStretch();

    // Re-create editors
    for (ComponentEditorCreator creator : m_componentEditorCreators) {
        creator(entity, m_mainWidget, m_mainLayout);
    }
}

void ComponentView::createConnections()
{
    connect(m_theSceneView, &SceneView::entityItemSelected,
            this, &ComponentView::setCurrentEntity);
}
