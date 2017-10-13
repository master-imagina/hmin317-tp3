#include "syncronizer.h"

#include <iostream>

Syncronizer * Syncronizer::instance = NULL;

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
        firstChanged(value % 4);
        secondChanged((value + 1) % 4);
        thirdChanged((value + 2) % 4);
        fourthChanged((value + 3) % 4);
        timer.restart();
    }
}