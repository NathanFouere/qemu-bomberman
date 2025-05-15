#include "Game.h"
#include <Applications/Utilities/PseudoRand.h>
#include <Applications/Board/Bomb.h>
#include <sextant/Activite/ThreadManager.h>

Game::Game(Clavier* k) : clavier(k) {
    ServiceLocator::provideKeyboard(k);
    lastFrameTime = 0;
}

Game::~Game() {
    // Resources will auto-clean via RAII
}

void Game::init() {
    set_vga_mode13();
    clear_vga_screen(16);
    set_palette_vga(palette_vga);
    
    int boardWidth = 20;
    int boardHeight = 11;
    
    // Create board using Resource
    board.reset(new Board(boardWidth, boardHeight));

    // Create player using Resource
    player1.reset(new Player(8, 40, PlayerType::PLAYER1, clavier, board.get()));
    player1->start();

    if (multiplayerMode) {
        player2.reset(new Player(8, 40, PlayerType::PLAYER2, clavier, board.get()));
        player2->start();
    }
    
    // Create bots using Resource
    for (int i = 0; i < MAX_BOTS; i++) {
        constexpr int EnemyTypeCount = 5;
        while (true) {
            int randX = pseudoRand() % (boardWidth - 2) + 1;
            int randY = pseudoRand() % (boardHeight - 2) + 1;

            int px = randX * TILE_SIZE + BOARD_ORIGIN_X;
            int py = randY * TILE_SIZE + BOARD_ORIGIN_Y;

            if (!board->isBlockedAt(px, py)) {
                bots[i].reset(new Bot(px, py, static_cast<EnemyType>(i % EnemyTypeCount), board.get()));
                bots[i]->start();
                break;
            }
        }
    }

    lastFrameTime = Timer::getInstance().getTicks();
}

void Game::update() {

    this->checkHitBomb(player1.get());

    if (multiplayerMode){
        this->checkHitBomb(player2.get());
    }
    
    for (int i = 0; i < MAX_BOTS; ++i) {
        this->checkHitBomb(bots[i].get());
    }
    ThreadManager::getInstance().yieldThread();
}

void Game::render() {

    clear_frame_buffer(228);

    plot_rectangle(0, 0, 24, 320, 5);
    draw_text("TIME", 4, 9, 15);
    // draw_number(timeRemaining, 68, 9, 15);

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
        if (bots[i].get() != nullptr && bots[i]->getStatus() != EntityStatus::DEAD) {
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
    while (true) {
        if (GameWin || GameOver) break;

        unsigned long frameStart = Timer::getInstance().getTicks();

        update();
        render();

        timeRemaining = TIME_LIMIT - Timer::getInstance().getSeconds();

        // Handle frame timing with ThreadManager
        unsigned long targetTime = frameStart + targetFrameTime;
        while (Timer::getInstance().getTicks() < targetTime) {
            ThreadManager::getInstance().yieldThread();
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