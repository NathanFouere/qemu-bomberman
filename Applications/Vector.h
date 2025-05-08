#pragma once

struct Vector {
    float x;
    float y;

    Vector() 
      : x(0.0f), y(0.0f)
    {}

    Vector(float xx, float yy) 
      : x(xx), y(yy)
    {}
};