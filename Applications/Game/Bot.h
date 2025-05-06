#include "Applications/Game/Movable.h"
#include "drivers/Clavier.h"
#include "sextant/Activite/Threads.h"

class Bot: public Threads, public Movable {
    public:
      int it = 0;
    void moveRandom();
    void run();
    Bot(int x, int y, unsigned char* data)
        : Movable(x, y, data) {}
};