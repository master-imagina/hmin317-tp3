#include "component.h"

#include "gameobject.h"

Component::Component(GameObject *parent) : gobject(parent) {}

GameObject& Component::gameObject() const {
    return *gobject;
}

void Component::update() {}

void Component::keyPressEvent(QKeyEvent * event) { event; }

void Component::keyReleaseEvent(QKeyEvent * event) { event; }

void Component::mousePressEvent(QMouseEvent * event) { event; }

void Component::mouseReleaseEvent(QMouseEvent * event) { event; }

void Component::mouseMoveEvent(QMouseEvent * event) { event; }

void Component::wheelEvent(QWheelEvent * event) { event; }

void Component::removeComponent() {
    gobject->removeComponent(this);
}
