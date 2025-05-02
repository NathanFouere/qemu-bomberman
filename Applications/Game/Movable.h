class Movable  {
private:
    int x, y;
    unsigned char* spriteData; // Pointeur vers les données du sprite
public:
    Movable() : x(0), y(0), spriteData(sprite_data) {} // Par défaut sprite_data
    Movable(int x, int y, unsigned char* data = sprite_data)
        : x(x), y(y), spriteData(data) {}

    void setSprite(unsigned char* data) { spriteData = data; }
    unsigned char* getSprite() const { return spriteData; }

    void move(int dx, int dy) { x += dx; y += dy; }
    int getX() const { return x; }
    int getY() const { return y; }
};
