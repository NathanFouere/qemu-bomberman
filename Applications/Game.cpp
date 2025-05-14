#include "Game.h"
#include <Applications/Utilities/PseudoRand.h>
#include <Applications/Board/Bomb.h>

Game::Game(Clavier* k) :clavier(k), player1(nullptr), player2(nullptr), board(nullptr) {
    for (int i = 0; i < MAX_BOTS; ++i) {
        bots[i] = nullptr;
    }
    lastFrameTime = 0;
}

void Game::init() {
    set_vga_mode13();
    clear_vga_screen(16);
    set_palette_vga(palette_vga);

    bool modeSelected = false;


    while (!modeSelected) {
        clear_frame_buffer(0);

        draw_text("BOMBERMAN", 120, 60, 15);
        draw_text("1. SOLO MODE", 100, 100, 15);
        draw_text("2. MULTIPLAYER", 100, 120, 15);
        draw_text("PRESS 1 OR 2 TO SELECT", 75, 160, 14);
        copy_frame_buffer_to_video();
        while (!clavier->testChar()) {
            thread_yield();
        }
        char key = clavier->getchar();
        if (key == '1') {
            multiplayerMode = false;
            modeSelected = true;
        } else if (key == '2') {
            multiplayerMode = true;
            modeSelected = true;
        }
    }
    // Display "STAGE 1"
    clear_frame_buffer(0);
    draw_text("STAGE 1", 135, 100, 15);
    copy_frame_buffer_to_video();

    unsigned int startStageTime = Timer::getInstance().getTicks();
    while (Timer::getInstance().getTicks() - startStageTime < 1500) {
        thread_yield();
    }

    board = new Board(BOARD_WIDTH, BOARD_HEIGHT);

    player1 = new Player(8, 40,PlayerType::PLAYER1, clavier, board);
    player1->start();

    if (multiplayerMode){
        player2 = new Player(8, 40,PlayerType::PLAYER2, clavier, board);
        player2->start();
    }
    
	for (int i = 0; i < MAX_BOTS; i++) {
        constexpr int EnemyTypeCount = 5;
        while (true) {
            int randX = pseudoRand() % (BOARD_WIDTH - 2) + 1;
            int randY = pseudoRand() % (BOARD_HEIGHT - 2) + 1;

            int px = randX * TILE_SIZE + BOARD_ORIGIN_X;
            int py = randY * TILE_SIZE + BOARD_ORIGIN_Y;

            if (!board->isBlockedAt(px, py)) {
                bots[i] = new Bot(px, py, static_cast<EnemyType>(i % EnemyTypeCount),board);
                bots[i]->start();
                break;
            }
        }
    }

    lastFrameTime = Timer::getInstance().getTicks();
    
    // Réinitialiser l'état du jeu à RUNNING
    gameState = GameState::RUNNING;
    waitingForKeyPress = false;
    exitWaitingLoop = false;
}

void Game::update() {
    // Skip normal update if we're in a waiting state
    if (waitingForKeyPress) {
        if (clavier->testChar()) {
            char key = clavier->getchar();
            if (key == Clavier::Enter) {
                waitingForKeyPress = false;
                this->resetGame();
            }
        }
        // Forced exit from waiting loop (timeout or exit request)
        if (exitWaitingLoop) {
            waitingForKeyPress = false;
            exitWaitingLoop = false;
            this->resetGame();
        }
        thread_yield();
        return;
    }

    this->checkHitBomb(player1);
    this->checkPlayerHitBot(player1);
    if (multiplayerMode){
        this->checkHitBomb(player2);
        this->checkPlayerHitBot(player2);
    }
    
    for (int i = 0; i < MAX_BOTS; ++i) {
        this->checkHitBomb(bots[i]);
    }

    checkGameWinAndLose();

    if (gameState == GameState::GAME_OVER) {
        clear_frame_buffer(0);
        draw_text("GAME OVER", 120, 80, 15);
        draw_text("PRESS ENTER TO RESTART", 70, 100, 15);
        copy_frame_buffer_to_video();
        
        // Set the waiting flag instead of blocking with a while loop
        waitingForKeyPress = true;
        
        // Set a timeout of 10 seconds to automatically restart if no key is pressed
        unsigned int startWaitTime = Timer::getInstance().getTicks();
        while (Timer::getInstance().getTicks() - startWaitTime < 10000) {
            if (clavier->testChar()) {
                char key = clavier->getchar();
                if (key == Clavier::Enter) {
                    waitingForKeyPress = false;
                    this->resetGame();
                    break;
                }
            }
            thread_yield();
        }
        // If we reach here, the timeout occurred
        waitingForKeyPress = false;
        this->resetGame();
    }
    else if (gameState == GameState::GAME_WIN) {
        clear_frame_buffer(0);
        draw_text("YOU WIN", 120, 60, 15);
        draw_text("PRESS ENTER TO RESTART", 100, 100, 15);
        copy_frame_buffer_to_video();
        
        // Set the waiting flag instead of blocking with a while loop
        waitingForKeyPress = true;
        
        // Set a timeout of 10 seconds to automatically restart if no key is pressed
        unsigned int startWaitTime = Timer::getInstance().getTicks();
        while (Timer::getInstance().getTicks() - startWaitTime < 10000) {
            if (clavier->testChar()) {
                char key = clavier->getchar();
                if (key == Clavier::Enter) {
                    waitingForKeyPress = false;
                    this->resetGame();
                    break;
                }
            }
            thread_yield();
        }
        // If we reach here, the timeout occurred
        waitingForKeyPress = false;
        this->resetGame();
    }

    thread_yield();
}

void Game::checkGameWinAndLose(){
    if (player1->getLives() <= 0) {
        gameState = GameState::GAME_OVER;
    }
    if (multiplayerMode && player2->getLives() <= 0 && player1->getLives() <= 0) {
        gameState = GameState::GAME_OVER;
    }

    if (timeRemaining <= 0) {
        gameState = GameState::GAME_OVER;
    }

    bool allBotsDead = true;
    for (int i = 0; i < MAX_BOTS; ++i) {
        if (bots[i] && bots[i]->getStatus() != EntityStatus::DEAD) {
            allBotsDead = false;
            break;
        }
    }
    if (allBotsDead) {
        gameState = GameState::GAME_WIN;
    }
}

void Game::render() {

    clear_frame_buffer(228);

    plot_rectangle(0, 0, 24, 320, 5);
    draw_text("TIME", 4, 9, 15);
    draw_number(timeRemaining, 68, 9, 15);

    // TODO get score + lives from player
    if (multiplayerMode){
        draw_number(player1->getScore(), 150, 9, 153);
        draw_number(player2->getScore(), 200, 9, 16);

        draw_text("LEFT", 250, 9, 15);
        draw_number(player1->getLives(), 300, 9, 153);
        draw_number(player2->getLives(), 312, 9, 16);

    } else {
        draw_number(player1->getScore(), 200, 9, 15);
        draw_text("LEFT", 250, 9, 15);
        draw_number(player1->getLives(), 300, 9, 15);
    }
    
    board->draw();

    for (int i = 0; i < MAX_BOTS; ++i) {
        if (bots[i] && bots[i]->getStatus() != EntityStatus::DEAD) {
            draw_sprite(bots[i]->getSprite(), 16, 16, bots[i]->getX(), bots[i]->getY());
        }
    }

    if (player1->getStatus() != EntityStatus::DEAD) {
        draw_sprite(player1->getSprite(), 16, 16, player1->getX(), player1->getY());
    }

    if (multiplayerMode && player2->getStatus() != EntityStatus::DEAD) {
        draw_sprite(player2->getSprite(), 16, 16, player2->getX(), player2->getY());
    }

    if (displayFPS) {
        unsigned long current = Timer::getInstance().getTicks();
        int fps = (current > lastFrameTime) ? (1000 / (current - lastFrameTime)) : 0;
        lastFrameTime = current;
        
        draw_text("FPS: ", 268, 188, 255);
        draw_number(fps, 316, 188, 255);
    }

    copy_frame_buffer_to_video();
}

void Game::run() {
    // Initialize the game state to RUNNING when we start the game
    gameState = GameState::RUNNING;

    while (true) {
        unsigned long frameStart = Timer::getInstance().getTicks();
        
        // Update the game
        update();
        
        // Only render if we're not waiting for a key press or if we are in a final state
        if (!waitingForKeyPress || gameState == GameState::GAME_OVER || gameState == GameState::GAME_WIN) {
            render();
        }

        // Update time remaining only in RUNNING state
        if (gameState == GameState::RUNNING) {
            timeRemaining = TIME_LIMIT - Timer::getInstance().getSeconds();
        }

        // Frame rate control
        unsigned long elapsed = Timer::getInstance().getTicks() - frameStart;
        while (elapsed < targetFrameTime) {
            // Give other threads a chance to run
            thread_yield();
            elapsed = Timer::getInstance().getTicks() - frameStart;
        }
    }
}

void Game::checkHitBomb(Movable* movable) {
    int startX = movable->getX();
    int startY = movable->getY();
    int endX = startX + TILE_SIZE;
    int endY = startY + TILE_SIZE;

    for (int x = startX; x < endX; ++x) {
        for (int y = startY; y < endY; ++y) {
            TileType tileType = this->board->getTileTypeAt(x, y);
            if (tileType == TILE_EXPLOSION) {
                movable->handleHitBomb();
                return;
            }
        }
    }
}    

void Game::checkPlayerHitBot(Player* player) {
    int playerX = player->getX();
    int playerY = player->getY();

    for (int i = 0; i < MAX_BOTS; ++i) {
        if (bots[i] && bots[i]->getStatus() != EntityStatus::DEAD) {

            int botX = bots[i]->getX();
            int botY = bots[i]->getY();

            if (playerX < botX + TILE_SIZE && playerX + TILE_SIZE > botX &&
                playerY < botY + TILE_SIZE && playerY + TILE_SIZE > botY) {
                player->decreaseLives();
                return;
            }
        }
    }
}

void Game::restartGame() {

}

void Game::resetGame() {
    // Nettoyer l'écran et afficher le message de redémarrage
    clear_frame_buffer(0);
    draw_text("RESTARTING GAME", 100, 100, 15);
    copy_frame_buffer_to_video();

    // Attendre un court instant pour que le joueur puisse lire le message
    unsigned int startStageTime = Timer::getInstance().getTicks();
    while (Timer::getInstance().getTicks() - startStageTime < 1500) {
        thread_yield();
    }
    
    // Libérer la mémoire des objets existants
    if (player1) {
        delete player1;
        player1 = nullptr;
    }
    
    if (player2) {
        delete player2;
        player2 = nullptr;
    }
    
    for (int i = 0; i < MAX_BOTS; ++i) {
        if (bots[i]) {
            delete bots[i];
            bots[i] = nullptr;
        }
    }
    
    if (board) {
        delete board;
        board = nullptr;
    }
    
    // Réinitialiser les variables d'état
    waitingForKeyPress = false;
    exitWaitingLoop = false;
    timeRemaining = TIME_LIMIT;

    // Initialiser un nouveau jeu
    init();
}

