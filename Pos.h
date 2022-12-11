//
// Created by thais on 10/12/22.
//

#ifndef ROBO_POS_H
#define ROBO_POS_H



class Pos {
public:
    int x{}, y{}, f{};
    unsigned long int time;

    Pos();

    Pos(int x, int y, int time, int f=-1);

    Pos &operator=(Pos);
};




#endif //ROBO_POS_H
