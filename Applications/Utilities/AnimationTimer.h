#ifndef ANIMATION_TIMER_H_
#define ANIMATION_TIMER_H_

#include <drivers/timer.h>

class AnimationTimer {
private:
    unsigned long lastUpdateTime;
    unsigned long updateInterval;
    
public:
    AnimationTimer(unsigned long interval = 150) : updateInterval(interval) {
        lastUpdateTime = Timer::getInstance().getTicks();
    }
    
    bool shouldUpdate() {
        unsigned long currentTime = Timer::getInstance().getTicks();
        if (currentTime - lastUpdateTime >= updateInterval) {
            lastUpdateTime = currentTime;
            return true;
        }
        return false;
    }
    
    void reset() {
        lastUpdateTime = Timer::getInstance().getTicks();
    }
    
    void setInterval(unsigned long interval) {
        updateInterval = interval;
    }
};

#endif /* ANIMATION_TIMER_H_ */