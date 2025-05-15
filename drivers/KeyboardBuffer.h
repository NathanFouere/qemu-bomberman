#ifndef KEYBOARD_BUFFER_H_
#define KEYBOARD_BUFFER_H_

#include <sextant/Synchronisation/Spinlock/Spinlock.h>

/**
 * Thread-safe keyboard buffer that can be accessed from both
 * interrupt handler and application code
 */
class KeyboardBuffer {
private:
    static const int BUFFER_SIZE = 256;
    static char buffer[BUFFER_SIZE];
    static int readPos;
    static int writePos;
    static int count;
    static Spinlock bufferLock;
    static int lockVar;

public:
    // Initialize the buffer
    static void init();
    
    // Put a character in the buffer (called from interrupt handler)
    static void put(char c);
    
    // Get a character from the buffer (called from application)
    static char get();
    
    // Check if buffer has data
    static bool hasData();
    
    // Clear the buffer
    static void clear();
};

#endif /* KEYBOARD_BUFFER_H_ */