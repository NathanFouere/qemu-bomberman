#include "Player.h"

Player::Player(const Vector& pos, unsigned char* data, Clavier* clavier) : Entity(pos), clavier(clavier) {
    // initialisation des données graphiques si nécessaire
}

void Player::update(float dt) {
    handleMovement(dt);
    handlePlaceBomb();
}

void Player::handleMovement(float dt) {
    Vector delta{0, 0};
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