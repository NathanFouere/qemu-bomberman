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
    clear_vga_screen(228);
    set_palette_vga(palette_vga);

    // TODO adjust spawn positions
    
    int boardWidth = 20;
	int boardHeight = 11;
	int botCount = 3;

    board = new Board(boardWidth, boardHeight);

    player1 = new Player(8, 40,PlayerType::PLAYER1, clavier, board);
    player1->start();

    if (multiplayerMode){
        player2 = new Player(8, 40,PlayerType::PLAYER2, clavier, board);
        player2->start();
    }

    // for (int i = 0; i < MAX_BOTS; ++i) {
    //     int x = 50 + (i % 5) * 25;
    //     int y = 50 + (i / 5) * 25;
    //     bots[i] = new Bot(x, y, static_cast<EnemyType>(i % EnemyTypeCount));
    //     bots[i]->start();
    // }
    
	for (int i = 0; i < MAX_BOTS; i++) {
        constexpr int EnemyTypeCount = 5;
        while (true) {
            int randX = pseudoRand() % (boardWidth - 2) + 1;
            int randY = pseudoRand() % (boardHeight - 2) + 1;

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
}

void Game::update() {
    thread_yield();
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

    while (true) {

        if (GameWin){
            break;
        }
        if (GameOver){
            break;
        }

        unsigned long frameStart = Timer::getInstance().getTicks();
        update();
        render();

        timeRemaining = TIME_LIMIT - Timer::getInstance().getSeconds();

        unsigned long elapsed = Timer::getInstance().getTicks() - frameStart;
        while (elapsed < targetFrameTime) {
            elapsed = Timer::getInstance().getTicks() - frameStart;
        }
    }
}
