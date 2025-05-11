#ifndef MOVABLE_H
#define MOVABLE_H
#include "Direction.h"

class Board;

class Movable  {
protected:
    int x, y;
    const unsigned char* sprites[4][3];
    int animationFrame;
    Direction direction;
public:
    Movable(int x, int y)
        : x(x), y(y), direction(RIGHT) {}

    const unsigned char* getSprite();

    void move(const Board& b, int dx, int dy);
    int getX() const { return x; }
    int getY() const { return y; }
};
#endif