#ifndef PANEMANAGER_H
#define PANEMANAGER_H

#include <qnamespace.h>


class MainWindow;
class Pane;

class QMenu;


class PaneManager
{
public:
    PaneManager(MainWindow *mainWindow, QMenu *viewPaneMenu);
    ~PaneManager() = default;

    void registerPane(Qt::DockWidgetArea area, Pane *pane);

private:
    MainWindow *m_mainWindow;
    QMenu *m_openViewPaneMenu;
};

#endif // PANEMANAGER_H
