#ifndef MOVABLE_H
#define MOVABLE_H

class Movable  {
protected:
    int x, y;
    unsigned char* spriteData;
public:
    Movable(int x, int y, unsigned char* data)
        : x(x), y(y), spriteData(data) {}

    void setSprite(unsigned char* data) { spriteData = data; }
    unsigned char* getSprite() const { return spriteData; }

    void move(int dx, int dy) { x += dx; y += dy; }
    int getX() const { return x; }
    int getY() const { return y; }
};
#endif