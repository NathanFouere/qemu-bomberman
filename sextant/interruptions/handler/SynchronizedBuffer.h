#include <sextant/Synchronisation/Mutex/Mutex.h>

class SynchronizedBuffer {
private:
    char data[256];
    int position;
    bool modified;
    Mutex accessMutex;
    
public:
    bool hasData() {
        accessMutex.lock();
        bool result = modified;
        accessMutex.unlock();
        return result;
    }
    
    char read() {
        accessMutex.lock();
        if (!modified) {
            accessMutex.unlock();
            return 0; 
        }
        
        char result = data[0];
        
        
        for (int i = 0; i < position - 1; i++) {
            data[i] = data[i + 1];
        }
        
        if (position > 0) position--;
        
        if (position == 0) {
            modified = false;
        }
        
        accessMutex.unlock();
        return result;
    }
    
    void write(char c) {
        accessMutex.lock();
        
        if (position < 255) {
            data[position] = c;
            position++;
            modified = true;
        }
        
        accessMutex.unlock();
    }
};