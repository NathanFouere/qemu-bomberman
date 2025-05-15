#ifndef MOVABLE_H
#define MOVABLE_H
#include "Direction.h"
#include "../Board/Board.h"
#include <drivers/timer.h>

enum class EntityStatus {
    ALIVE,
    DEAD,
    DEAD_ANIMATION // TODO => pas sur qu'on garde ce statut
};

const int DEATH_ANIMATION_TIME = 2;

class Movable  {
protected:
    int x, y;
    const unsigned char* sprites[4][3];
    const unsigned char* deathSprites[5];
    int animationFrame;
    Direction direction;
    EntityStatus status;
    int deathAnimationStartTime = 0;
public:
    Movable(int x, int y)
        : x(x), y(y), direction(RIGHT) {}

    const unsigned char* getSprite();

    void move(const Board& b, int dx, int dy);
    int getX() const { return x; }
    int getY() const { return y; }
    EntityStatus getStatus() const { return status; }
    void setStatus(EntityStatus newStatus) { status = newStatus; }
    void render();
};
#endif