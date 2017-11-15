#include "panemanager.h"

#include <QtWidgets/QMenu>

#include "gui/pane.h"

#include "mainwindow.h"


PaneManager::PaneManager(MainWindow *mainWindow, QMenu *viewPaneMenu) :
    m_mainWindow(mainWindow),
    m_openViewPaneMenu(viewPaneMenu)
{}

void PaneManager::registerPane(Qt::DockWidgetArea area, Pane *pane)
{
    m_openViewPaneMenu->addAction(pane->toggleViewAction());

    m_mainWindow->addDockWidget(area, pane);
}
