#include "Applications/Game/Movable.h"
#include "drivers/Clavier.h"
#include "sextant/Activite/Threads.h"

class Player: public Threads, public Movable {
    private:
        Clavier* clavier;
    public:
        
        void movePlayer();
        Player(int x, int y, unsigned char* data, Clavier* clavier)
            : Movable(x, y, data), clavier(clavier) {}
        void run();
};