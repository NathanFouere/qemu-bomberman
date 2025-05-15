#include "Clavier.h"
#include <sextant/ordonnancements/preemptif/thread.h>
#include <drivers/KeyboardBuffer.h>

bool Clavier::testChar() {
    return KeyboardBuffer::hasData();
}

char Clavier::getchar() {
    if (!KeyboardBuffer::hasData()) {
        return 0;
    }
    
    return KeyboardBuffer::get();
}

char Clavier::waitChar() {
    // Wait until a key is available
    while (!KeyboardBuffer::hasData()) {
        thread_yield();
    }
    
    return KeyboardBuffer::get();
}

void Clavier::set_leds(void) {
    ecrireOctet(0x60, 0xED);
}