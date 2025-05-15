#ifndef THREAD_MANAGER_H_
#define THREAD_MANAGER_H_

#include <sextant/ordonnancements/preemptif/thread.h>
#include <drivers/timer.h>

class ThreadManager {
private:
    static ThreadManager* instance;
    
public:
    static ThreadManager& getInstance() {
        if (!instance) instance = new ThreadManager();
        return *instance;
    }
    
    // Sleep with yielding
    void sleep(unsigned long milliseconds) {
        unsigned long start = Timer::getInstance().getTicks();
        unsigned long target = start + milliseconds;
        
        while (Timer::getInstance().getTicks() < target) {
            thread_yield();
        }
    }
    
    void yieldThread() {
        thread_yield();
    }
};

#endif /* THREAD_MANAGER_H_ */