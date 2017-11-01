#include "dragtoolbutton.h"

#include <QtGui/QMouseEvent>

/*!
    \class DragToolButton

    The DragToolButton class provides a button with infinite "press and move".
*/

/*!
    Constructs a DragToolButton with the parent \p parent.
*/
DragToolButton::DragToolButton(QWidget *parent) :
    QToolButton(parent),
    m_lastMousePosOnPress(),
    m_lastMousePos()
{
    setCheckable(true);
}

/*!
    Destroys the DragToolButton.
*/
DragToolButton::~DragToolButton()
{}

/*!
    Reimplemented from QWidget::mousePressEvent().
*/
void DragToolButton::mousePressEvent(QMouseEvent *e)
{
    setChecked(true);

    m_lastMousePosOnPress = m_lastMousePos = e->pos();
}

/*!
    Reimplemented from QWidget::mouseMoveEvent().
*/
void DragToolButton::mouseMoveEvent(QMouseEvent *e)
{
    setCursor(Qt::BlankCursor);

    const QPoint newMousePos = e->pos();

    const QPoint mouseOffset = m_lastMousePos - newMousePos;
    const float offsetX = mouseOffset.x();
    const float offsetY = mouseOffset.y();

    Q_EMIT mouseDragged(offsetX, offsetY);

    QCursor::setPos(mapToGlobal(m_lastMousePosOnPress));

    m_lastMousePos = m_lastMousePosOnPress;
}

/*!
    Reimplemented from QWidget::mouseReleaseEvent().
*/
void DragToolButton::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);

    setChecked(false);

    setCursor(Qt::ArrowCursor);
}
