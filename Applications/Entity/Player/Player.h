#pragma once
#include "../Entity.h"
#include "drivers/Clavier.h"

/**
 * Player dérive de Entity et gère son déplacement,
 * la pose de bombes et la réception d'input via Clavier.
 */
class Player : public Entity {
public:
    /**
     * Constructeur:
     * @param pos position initiale
     * @param data pointeur vers le sprite ou les données graphiques
     * @param clavier pointeur vers le gestionnaire de touches
     */
    Player(const Vector& pos, unsigned char* data, Clavier* clavier);

    void update(float dt) override;

private:
    Clavier* clavier;
    int maxBombs{1};
    float speed{100.f};

    void handleMovement(float dt);
    void handlePlaceBomb();
};