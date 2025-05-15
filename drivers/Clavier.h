#ifndef CLAVIER_H_
#define CLAVIER_H_

#include <hal/fonctionsES.h>
#include <sextant/Synchronisation/Mutex/Mutex.h>

/**
 * @file Clavier.h
 * @class Clavier
 * @brief Permet la récupération des caractères saisis au clavier.
 */

class Clavier {
private:
    static Mutex keyboardMutex;  // Add mutex for synchronization

public:
    bool testChar();
    char getchar();
    char* getString();
    void set_leds(void);
};

#endif /* CLAVIER_H_ */
