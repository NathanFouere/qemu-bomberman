#ifndef GAME_H_
#define GAME_H_

#include <drivers/timer.h>
#include <drivers/Clavier.h>
#include <Applications/Entity/Bot.h>
#include <Applications/Entity/Player.h>
#include <Applications/Board/Board.h>
#include "Ressource.h"
#include "ServiceLocator.h"

#define MAX_BOTS 5
#define TARGET_FPS 60
#define TIME_LIMIT 200

class Game {
public:
    Game(Clavier* k);
    ~Game(); // Add destructor for cleanup
    void init();
    void update();
    void render();
    void run();

    bool displayFPS = true;
    bool multiplayerMode = false;

private:
    Clavier* clavier;  // Raw pointer since we don't own this

    Resource<Player> player1;
    Resource<Player> player2;
    Resource<Bot> bots[MAX_BOTS];
    Resource<Board> board;

    int timeRemaining = TIME_LIMIT;
    
    bool GameOver = false;
    bool GameWin = false;
    bool GameRestart = false;
    bool GameReset = false;

    unsigned long lastFrameTime;
    const int targetFrameTime = 1000 / TARGET_FPS;

    void checkHitBomb(Movable* movable);
};

#endif /* GAME_H_ */