#ifndef PANE_H
#define PANE_H

#include <QDockWidget>

class QScrollArea;
class QToolBar;


class Pane : public QDockWidget
{
public:
    explicit Pane(const QString &title, QWidget *parent = nullptr);
    virtual ~Pane();

    QScrollArea *placeholder() const;

    QWidget *widget() const;
    void setWidget(QWidget *widget);

    void addToolBar(Qt::ToolBarArea area, QToolBar *toolbar);

private:
    QMainWindow *m_placeholder;
    QScrollArea *m_scrollArea;
};

#endif // PANE_H
