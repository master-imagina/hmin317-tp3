#include "inputcontroller.h"
#include <QKeyEvent>
#include <iostream>

InputController::InputController()
{

}

void InputController::keyPressed(int qtkey){
    switch(qtkey){
    case Qt::Key_Z : KEY_Z = true; break;
    case Qt::Key_Q : KEY_Q = true; break;
    case Qt::Key_D : KEY_D = true; break;
    case Qt::Key_S : KEY_S = true; break;
    case Qt::Key_Shift : KEY_SHIFT = true; break;
    case Qt::Key_Space : KEY_SPACE = true; break;
    default: break;
    }

}

void InputController::keyReleased(int qtkey){
    switch(qtkey){
    case Qt::Key_Z : KEY_Z = false; break;
    case Qt::Key_Q : KEY_Q = false; break;
    case Qt::Key_D : KEY_D = false; break;
    case Qt::Key_S : KEY_S = false; break;
    case Qt::Key_Shift : KEY_SHIFT = false; break;
    case Qt::Key_Space : KEY_SPACE = false; break;

    default: break;
    }

}


void InputController::mousePressed(int qtmouse){
    switch(qtmouse){
        case Qt::LeftButton : MOUSE_LEFTBUTTON = true; break;
        default: break;
    }
}

void InputController::mouseReleased(int qtmouse){
    switch(qtmouse){
    case Qt::LeftButton : MOUSE_LEFTBUTTON = false; break;
    default: break;
    }
}

bool InputController::getKeyZ() const{
    return KEY_Z;

}
bool InputController::getKeyS() const{
    return KEY_S;
}
bool InputController::getKeyQ() const{
    return KEY_Q;
}
bool InputController::getKeyD() const{
    return KEY_D;
}
bool InputController::getKeyShift() const{
    return KEY_SHIFT;
}
bool InputController::getKeySpace() const{
    return KEY_SPACE;
}

bool InputController::getMouseLeftButton() const{
    return MOUSE_LEFTBUTTON;
}


void InputController::setKeyZ(bool b){
    KEY_Z = b;
}
void InputController::setKeyS(bool b){
    KEY_S = b;
}
void InputController::setKeyQ(bool b) {
    KEY_Q = b;
}
void InputController::setKeyD(bool b){
    KEY_D = b;
}
