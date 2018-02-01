#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "input/eventhook.h"


struct Keyboard
{
    Keyboard() :
        m_theEventHook(nullptr)
    {}

    bool keyIsPressed(int keyCode) const
    {
        return m_theEventHook->keyIsPressed(keyCode);
    }

    EventHook *m_theEventHook;
};

#endif // KEYBOARD_H
