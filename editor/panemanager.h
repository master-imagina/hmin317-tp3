#ifndef PANEMANAGER_H
#define PANEMANAGER_H

#include <qnamespace.h>

class MainWindow;

class QDockWidget;
class QMenu;


class PaneManager
{
public:
    PaneManager(MainWindow *mainWindow, QMenu *viewPaneMenu);
    ~PaneManager() = default;

    void registerPane(Qt::DockWidgetArea area, QDockWidget *pane,
                      bool open = true);

private:
    MainWindow *m_mainWindow;
    QMenu *m_openViewPaneMenu;
};

#endif // PANEMANAGER_H
