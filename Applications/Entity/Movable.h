#ifndef MOVABLE_H
#define MOVABLE_H
#include "Direction.h"

enum class EntityStatus {
    ALIVE,
    DEAD,
    DEAD_ANIMATION
};

class Board;

class Movable  {
protected:
    int x, y;
    const unsigned char* sprites[4][3];
    int animationFrame;
    Direction direction;
    EntityStatus status;
public:
    Movable(int x, int y)
        : x(x), y(y), direction(RIGHT) {}

    const unsigned char* getSprite();

    void move(const Board& b, int dx, int dy);
    int getX() const { return x; }
    int getY() const { return y; }
    EntityStatus getStatus() const { return status; }
    void setStatus(EntityStatus newStatus) { status = newStatus; }
};
#endif