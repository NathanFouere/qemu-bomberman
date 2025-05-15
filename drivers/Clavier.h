#ifndef CLAVIER_H_
#define CLAVIER_H_

#include <hal/fonctionsES.h>
#include <drivers/KeyboardBuffer.h>

/**
 * @file Clavier.h
 * @class Clavier
 * @brief Keyboard interface for user applications
 */
class Clavier {
public:
    // Check if a key is available
    bool testChar();
    
    // Get a character (returns 0 if no key available)
    char getchar();
    
    // Wait for a key and return it
    char waitChar();
    
    // Set keyboard LEDs (not implemented)
    void set_leds(void);
};

#endif /* CLAVIER_H_ */
