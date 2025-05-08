
#pragma once
#include "../Vector.h"

// Root class for all entities in the game
class Entity {
public:
    Entity(const Vector& pos) : position(pos) {}
    virtual ~Entity() = default;

    
    virtual void update(float dt) = 0;

    Vector position;
};