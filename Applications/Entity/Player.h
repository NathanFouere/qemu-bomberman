#include "drivers/Clavier.h"
#include "sextant/Activite/Threads.h"
#include "Direction.h"
#include <drivers/sprite.h>
#include "Applications/Entity/Movable.h"

class Player: public Threads, public Movable {
    private:
        Clavier* clavier;
    public:
        Player(int x, int y, Clavier* clavier)
            : Movable(x, y), clavier(clavier) {
                sprites[UP][0] = player1_back_1;
                sprites[UP][1] = player1_back_2;
                sprites[UP][2] = player1_back_3;
        
                sprites[DOWN][0] = player1_front_1;
                sprites[DOWN][1] = player1_front_2;
                sprites[DOWN][2] = player1_front_3;
        
                sprites[LEFT][0] = player1_left_1;
                sprites[LEFT][1] = player1_left_2;
                sprites[LEFT][2] = player1_left_3;
        
                sprites[RIGHT][0] = player1_right_1;
                sprites[RIGHT][1] = player1_right_2;
                sprites[RIGHT][2] = player1_right_3;
            }
        void movePlayer();
        void run();
        void render();
        int getX() const { return x; }
        int getY() const { return y; }
};
