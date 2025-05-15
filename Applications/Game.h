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
#define FREEZE_DURATION 1000 // Duration in milliseconds for which the game freezes when player is hit

#define BOARD_WIDTH 20
#define BOARD_HEIGHT 11

enum class GameState {
    INIT,
    RUNNING,
    GAME_OVER,
    GAME_WIN,
    GAME_RESTART,
    GAME_RESET
};

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
    
    // Returns true if any player is in death animation
    bool isPlayerInDeathAnimation() const;

private:
    Clavier* clavier;  // Raw pointer since we don't own this

    Resource<Player> player1;
    Resource<Player> player2;
    Resource<Bot> bots[MAX_BOTS];
    Resource<Board> board;

    int timeRemaining = TIME_LIMIT;
    unsigned long gameStartTime; // Time when the game actually starts
    unsigned long playerHitTime; // Time when the player was hit

    GameState gameState = GameState::INIT;

    unsigned long lastFrameTime;
    const int targetFrameTime = 1000 / TARGET_FPS;

    void checkPlayerHitBot(Player* player);

    void resetGame();
    void restartGame();
    void checkGameWinAndLose();

    void checkHitBombBot(Bot* movable);
    void checkHitBombPlayer(Player* movable);
};

#endif /* GAME_H_ */