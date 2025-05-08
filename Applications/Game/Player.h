#include "Applications/Game/Movable.h"
#include "drivers/Clavier.h"
#include "sextant/Activite/Threads.h"
#include "Direction.h"
#include <drivers/sprite.h>

class Player: public Threads, public Movable {
    private:
        int x,y;
        Clavier* clavier;
    public:
        
        void movePlayer();
        Player(int x, int y, Clavier* clavier)
            : Movable(x, y), clavier(clavier) {
                sprites[UP][0] = player1_back_1;
                sprites[UP][1] = player1_back_1;
                sprites[UP][2] = player1_back_1;
        
                sprites[DOWN][0] = player1_front_1;
                sprites[DOWN][1] = player1_front_1;
                sprites[DOWN][2] = player1_front_1;
        
                sprites[LEFT][0] = player1_left_1;
                sprites[LEFT][1] = player1_left_1;
                sprites[LEFT][2] = player1_left_1;
        
                sprites[RIGHT][0] = player1_right_1;
                sprites[RIGHT][1] = player1_right_1;
                sprites[RIGHT][2] = player1_right_1;
            }
        void run();
        void render();
        int getX() const { return x; }
        int getY() const { return y; }
};
