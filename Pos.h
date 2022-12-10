//
// Created by thais on 10/12/22.
//

#ifndef PROJETO2SO_POS_H
#define PROJETO2SO_POS_H



class Pos {
public:
    int x{}, y{}, f{};
    unsigned long int time;

    Pos();

    Pos(int x, int y, int time, int f=-1);

    Pos &operator=(Pos);
};




#endif //PROJETO2SO_POS_H
