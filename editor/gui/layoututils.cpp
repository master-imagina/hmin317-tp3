#include "layoututils.h"

#include <QLayout>
#include <QWidget>


void clearLayout(QLayout *layout, int from)
{
    QLayoutItem *childItem = nullptr;

    while ( (childItem = layout->takeAt(from)) ) {
        QWidget *childWidget = childItem->widget();

        if (childWidget) {
            childWidget->deleteLater();
        }

        delete childItem;
    }
}
