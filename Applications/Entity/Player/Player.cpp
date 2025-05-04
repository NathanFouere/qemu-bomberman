#include "Player.h"

Player::Player(const Vector2& pos, unsigned char* data, Clavier* clavier)
    : Entity(pos), clavier(clavier) {
    // initialisation des données graphiques si nécessaire
}

void Player::update(float dt) {
    handleMovement(dt);
    handlePlaceBomb();
}

void Player::render() {
    // appel au Renderer ou draw direct selon implémentation
}

void Player::handleMovement(float dt) {
    Vector2 delta{0, 0};
    if (clavier->isPressed(Clavier::Up))    delta.y -= speed * dt;
    if (clavier->isPressed(Clavier::Down))  delta.y += speed * dt;
    if (clavier->isPressed(Clavier::Left))  delta.x -= speed * dt;
    if (clavier->isPressed(Clavier::Right)) delta.x += speed * dt;

    position.x += delta.x;
    position.y += delta.y;
}

void Player::handlePlaceBomb() {
    if (clavier->isPressed(Clavier::PlaceBomb) && maxBombs > 0) {
        // appeler directement la création de bombe via Game (à implémenter)
        --maxBombs;
    }
}