#include "KeyboardBuffer.h"

// Define static members
char KeyboardBuffer::buffer[BUFFER_SIZE];
int KeyboardBuffer::readPos = 0;
int KeyboardBuffer::writePos = 0;
int KeyboardBuffer::count = 0;
Spinlock KeyboardBuffer::bufferLock;
int KeyboardBuffer::lockVar = 1;

void KeyboardBuffer::init() {
    readPos = 0;
    writePos = 0;
    count = 0;
}

void KeyboardBuffer::put(char c) {
    // Use spinlock for interrupt context
    bufferLock.Take(&lockVar);
    
    if (count < BUFFER_SIZE) {
        buffer[writePos] = c;
        writePos = (writePos + 1) % BUFFER_SIZE;
        count++;
    }
    
    bufferLock.Release(&lockVar);
}

char KeyboardBuffer::get() {
    char result = 0;
    
    bufferLock.Take(&lockVar);
    
    if (count > 0) {
        result = buffer[readPos];
        readPos = (readPos + 1) % BUFFER_SIZE;
        count--;
    }
    
    bufferLock.Release(&lockVar);
    
    return result;
}

bool KeyboardBuffer::hasData() {
    bufferLock.Take(&lockVar);
    bool result = (count > 0);
    bufferLock.Release(&lockVar);
    return result;
}

void KeyboardBuffer::clear() {
    bufferLock.Take(&lockVar);
    readPos = 0;
    writePos = 0;
    count = 0;
    bufferLock.Release(&lockVar);
}