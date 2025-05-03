#include "Applications/Game/Movable.h"

class Bot : public Movable {
    public:
        Bot(int x, int y, unsigned char* data)
            : Movable(x, y, data) {}

        void moveRandomly();
};