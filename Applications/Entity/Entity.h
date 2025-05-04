
#pragma once
#include "Vector.h"
#include <memory>
#include <vector>

// Root class for all entities in the game
class Entity {
public:
    Entity(const Vector2& pos) : position(pos) {}
    virtual ~Entity() = default;

    
    virtual void update(float dt) = 0;
    virtual void render() = 0;
    virtual void onCollision(const std::shared_ptr<Entity>& other) = 0;

    Vector2 position;
};