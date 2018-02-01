#include "eventhook.h"

#include <QDebug>
#include <QKeyEvent>


EventHook::EventHook(QObject *parent) :
    QObject(parent),
    m_keyTable()
{}

bool EventHook::keyIsPressed(int key) const
{
    const auto keyFound = m_keyTable.find(key);

    if (keyFound != m_keyTable.end()) {
        return keyFound->second;
    }

    return false;
}

void EventHook::clear()
{
    m_keyTable.clear();
}

bool EventHook::eventFilter(QObject *obj, QEvent *e)
{
    const QEvent::Type eventType = e->type();

    if (eventType == QEvent::KeyPress) {
        auto keyEvent = static_cast<QKeyEvent *>(e);

        keyPressEvent(keyEvent);

        return false;
    }
    else if (eventType == QEvent::KeyRelease) {
        auto keyEvent = static_cast<QKeyEvent *>(e);

        keyReleaseEvent(keyEvent);

        return false;
    }

    return QObject::eventFilter(obj, e);
}

void EventHook::keyPressEvent(QKeyEvent *e)
{
    const int keyCode = e->key();

    if (keyCode != 0 && keyCode != Qt::Key_unknown) {
        m_keyTable[keyCode] = true;
    }
}

void EventHook::keyReleaseEvent(QKeyEvent *e)
{
    const int keyCode = e->key();

    if (keyCode != 0 && keyCode != Qt::Key_unknown) {
        m_keyTable[keyCode] = false;
    }
}

