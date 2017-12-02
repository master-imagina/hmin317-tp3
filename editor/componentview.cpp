#include "componentview.h"

#include <QBoxLayout>
#include <QDebug>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QToolButton>

#include "core/scene.h"

#include "editor/projectmanager.h"
#include "editor/sceneview.h"

#include "editor/gui/layoututils.h"


ComponentView::ComponentView(SceneView *sceneView,
                             ProjectManager *projectManager,
                             QWidget *parent) :
    ScrollablePane(tr("Components"), parent),
    m_theSceneView(sceneView),
    m_theProjectManager(projectManager),
    m_mainWidget(nullptr),
    m_mainLayout(nullptr),
    m_currentEntity(),
    m_componentUiHandlers()
{
    Q_ASSERT (m_theSceneView);

    m_mainWidget = new QWidget(this);
    setWidget(m_mainWidget);

    auto *addComponentBtn = new QToolButton(m_mainWidget);
    addComponentBtn->setText(tr("Add Component"));
    addComponentBtn->setPopupMode(QToolButton::InstantPopup);
    addComponentBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_addComponentMenu = new QMenu(addComponentBtn);

    addComponentBtn->setMenu(m_addComponentMenu);

    m_mainLayout = new QVBoxLayout(m_mainWidget);
    m_mainLayout->addWidget(addComponentBtn);
    m_mainLayout->addStretch();

    createConnections();
}

void ComponentView::setCurrentEntity(entityx::Entity entity)
{
    m_currentEntity = entity;

    clearLayout(m_mainLayout, 1);

    m_mainLayout->addStretch();

    const bool entityIsValid = entity.valid();

    m_addComponentMenu->setEnabled(entityIsValid);

    // Re-create editors
    if (entityIsValid) {
        for (uptr<IComponentUiHandler> &compUiHandler : m_componentUiHandlers) {
            const bool hasComponent = compUiHandler->hasComponent(m_currentEntity);

            if (hasComponent) {
                auto *compEditorPlaceholder = new QWidget(m_mainWidget);

                auto *compTitleBtn = new QPushButton(compEditorPlaceholder);
                compTitleBtn->setCheckable(true);
                compTitleBtn->setChecked(true);
                compTitleBtn->setText(compUiHandler->componentName());

                QWidget *compEditor =
                        compUiHandler->createComponentEditor(m_currentEntity,
                                                             compEditorPlaceholder,
                                                             m_theProjectManager->currentProjectPath());

                connect(compTitleBtn, &QPushButton::toggled,
                        compEditor, &QWidget::setVisible);

                auto *fullEditorPlaceholderLayout = new QVBoxLayout(compEditorPlaceholder);
                fullEditorPlaceholderLayout->addWidget(compTitleBtn);
                fullEditorPlaceholderLayout->addWidget(compEditor);

                m_mainLayout->insertWidget(m_mainLayout->count() - 1, compEditorPlaceholder);
            }
        }
    }

    updateComponentMenuForEntity();
}

void ComponentView::createConnections()
{
    connect(m_theSceneView, &SceneView::entityItemSelected,
            this, &ComponentView::setCurrentEntity);
}

void ComponentView::updateComponentMenuForEntity()
{
    m_addComponentMenu->clear();

    for (uptr<IComponentUiHandler> &compUiHandler : m_componentUiHandlers) {
        QAction *addComponentAction =
                m_addComponentMenu->addAction(compUiHandler->componentName());

        const bool hasComponent = compUiHandler->hasComponent(m_currentEntity);

        addComponentAction->setDisabled(hasComponent);

        compUiHandler->configureAddAction(m_currentEntity, addComponentAction);
    }
}
