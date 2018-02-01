#include "pane.h"

#include <QMainWindow>
#include <QScrollArea>
#include <QToolBar>


ScrollablePane::ScrollablePane(const QString &title, QWidget *parent) :
    QDockWidget(title, parent),
    m_placeholder(new QMainWindow(this)),
    m_scrollArea(new QScrollArea(m_placeholder))
{
    m_placeholder->setWindowFlags(m_placeholder->windowFlags() & ~Qt::Window);

    QDockWidget::setWidget(m_placeholder);

    m_scrollArea->setWidgetResizable(true);

    m_placeholder->setCentralWidget(m_scrollArea);
}

ScrollablePane::~ScrollablePane()
{}

QScrollArea *ScrollablePane::placeholder() const
{
    return m_scrollArea;
}

QWidget *ScrollablePane::widget() const
{
    return m_scrollArea->widget();
}

void ScrollablePane::setWidget(QWidget *widget)
{
    if (m_scrollArea->widget() != widget) {
        m_scrollArea->setWidget(widget);
    }
}

void ScrollablePane::addToolBar(Qt::ToolBarArea area, QToolBar *toolbar)
{
    m_placeholder->addToolBar(area, toolbar);
}
