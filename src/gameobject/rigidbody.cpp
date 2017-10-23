#include "rigidbody.h"
#include "gameobject.h"

Rigidbody::Rigidbody(GameObject *parent) :
    Component(parent),
    gravity(0.0f, -1.0f, 0.0f)
{

}

void Rigidbody::update() {
    if (!timer.isValid()) {
        timer.start();
        return;
    }
    float time = (float)timer.elapsed() / 1000.0f;
    velocity += gravity * (9.81f * time);
    gameObject().transform().position += velocity * time;
    timer.restart();
}

void Rigidbody::destroy() {
    removeComponent();
    delete this;
}

int Rigidbody::component() const {
    return COMPONENT_RIGIDBODY_ID;
}

void Rigidbody::clone(GameObject *c) {
    Rigidbody * r = c->addComponent<Rigidbody>();
    r->gravity = gravity;
    r->velocity = velocity;
}

bool Rigidbody::isInstance(Component *c) {
    return (c->component() == COMPONENT_RIGIDBODY_ID);
}
