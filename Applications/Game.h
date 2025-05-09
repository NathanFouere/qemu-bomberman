#include <drivers/timer.h>
#include <drivers/Clavier.h>
#include <Applications/Entity/Bot.h>
#include <Applications/Entity/Player.h>
#include <Applications/Board/Board.h>


#define MAX_BOTS 5
#define TARGET_FPS 60
#define TIME_LIMIT 200

class Game {
public:
    Game(Timer* t, Clavier* k);
    void init();
    void update();
    void render();
    void run();

    bool displayFPS = true;




private:
    Timer* timer;
    Clavier* clavier;

    Player* player;
    Bot* bots[MAX_BOTS];
    Board* board;

    int timeRemaining = TIME_LIMIT;
    
    bool displayGameOver = true;
    bool displayGameWon = true;

    unsigned long lastFrameTime;
    const int targetFrameTime = 1000 / TARGET_FPS;
};
