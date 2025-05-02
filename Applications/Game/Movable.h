class Movable  {
private:
    int x, y;
    unsigned char* spriteData; // Pointeur vers les données du sprite
public:
    Movable() : x(0), y(0), spriteData(bomb_1) {} // Par défaut sprite_data
    Movable(int x, int y, unsigned char* data = bomb_1)
        : x(x), y(y), spriteData(data) {}

    void setSprite(unsigned char* data) { spriteData = data; }
    unsigned char* getSprite() const { return spriteData; }

    void move(int dx, int dy) { x += dx; y += dy; }
    int getX() const { return x; }
    int getY() const { return y; }
};
