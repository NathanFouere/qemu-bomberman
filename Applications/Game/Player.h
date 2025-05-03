#include "Applications/Game/Movable.h"
#include "drivers/Clavier.h"

class Player : public Movable {
    private:
        Clavier* clavier;
    public:
        void movePlayer();
        Player(int x, int y, unsigned char* data, Clavier* clavier)
            : Movable(x, y, data), clavier(clavier) {}
};