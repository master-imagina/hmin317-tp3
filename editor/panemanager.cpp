#include "panemanager.h"

#include <QDockWidget>
#include <QMenu>

#include "mainwindow.h"


PaneManager::PaneManager(MainWindow *mainWindow, QMenu *viewPaneMenu) :
    m_mainWindow(mainWindow),
    m_openViewPaneMenu(viewPaneMenu)
{}

void PaneManager::registerPane(Qt::DockWidgetArea area, QDockWidget *pane,
                               bool open)
{
    QAction *toggleViewAction = pane->toggleViewAction();

    m_openViewPaneMenu->addAction(toggleViewAction);

    m_mainWindow->addDockWidget(area, pane);

    if (!open) {
        pane->hide();
    }
}
