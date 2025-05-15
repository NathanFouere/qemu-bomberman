#include "Clavier.h"
#include <sextant/ordonnancements/preemptif/thread.h>  // Add this include for thread_yield

extern bool modifBuf;
extern char buf[256];
extern int posBuf;

// Initialize the static mutex
Mutex Clavier::keyboardMutex;

bool Clavier::testChar() {
    keyboardMutex.lock();
    bool result = modifBuf;
    keyboardMutex.unlock();
    return result;
}

char Clavier::getchar() {
    keyboardMutex.lock();
    
    while(!modifBuf) {
        keyboardMutex.unlock();
        thread_yield();
        keyboardMutex.lock();
    }
    
    char result = buf[0];
    
    // Move remaining characters in buffer
    for(int i = 0; i < posBuf - 1; i++) {
        buf[i] = buf[i+1];
    }
    
    if(posBuf > 0) posBuf--;
    
    if(posBuf == 0) {
        modifBuf = false;
    }
    
    keyboardMutex.unlock();
    return result;
}

char* Clavier::getString() {
    keyboardMutex.lock();
    while(!modifBuf) {
        keyboardMutex.unlock();
        thread_yield();  // Release mutex while waiting for input
        keyboardMutex.lock();
    }
    
    modifBuf = false;
    char* result = buf;
    posBuf = 0;
    keyboardMutex.unlock();
    return result;
}

void Clavier::set_leds(void) {
    ecrireOctet(0x60, 0xED);
}