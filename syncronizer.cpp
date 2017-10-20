#include "syncronizer.h"

#include <iostream>

Syncronizer * Syncronizer::instance = NULL;

float Syncronizer::delay = 3.0;

Syncronizer::Syncronizer() :
    value(0)
{
    timer.start();
    instance = this;
}

Syncronizer& Syncronizer::getInstance() {
    return *instance;
}

void Syncronizer::update() {
    if ((float)timer.elapsed() / 1000 > delay) {
        std::cout << "--- UPDATE ---" << std::endl;
        value = (value + 1) % 4;
        emit firstChanged(value % 4);
        emit secondChanged((value + 1) % 4);
        emit thirdChanged((value + 2) % 4);
        emit fourthChanged((value + 3) % 4);
        timer.restart();
    }
}
