#ifndef H_DRAGTOOLBUTTON_H
#define H_DRAGTOOLBUTTON_H

#include <QToolButton>


class DragToolButton : public QToolButton
{
    Q_OBJECT

public:
    explicit DragToolButton(QWidget *parent = nullptr);
    ~DragToolButton();

Q_SIGNALS:
    void mouseDragged(float dragX, float dragY);

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

private:
    QPoint m_lastMousePosOnPress;
    QPoint m_lastMousePos;
};

#endif // H_DRAGTOOLBUTTON_H
