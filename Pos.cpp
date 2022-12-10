//
// Created by thais on 10/12/22.
//

#include "Pos.h"

Pos::Pos(int x, int y, int time, int f) {
    this->x = x;
    this->y = y;
    this->time = time;
    this->f = f;
}


Pos::Pos() {
    x = -1;
    y = -1;
    time = -1;
    f = -1;
}


Pos &Pos::operator=(Pos outra) {
    this->x = outra.x;
    this->y = outra.y;
    this->time = outra.time;
    this->f = outra.f;
    return *this;
}
