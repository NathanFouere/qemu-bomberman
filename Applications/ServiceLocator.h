#ifndef SERVICE_LOCATOR_H_
#define SERVICE_LOCATOR_H_

#include <drivers/Clavier.h>
#include <drivers/timer.h>

class ServiceLocator {
private:
    static Clavier* keyboardService;
    
public:
    static void provideKeyboard(Clavier* keyboard) {
        keyboardService = keyboard;
    }
    
    static Clavier& getKeyboard() {
        return *keyboardService;
    }
    
    static Timer& getTimer() {
        return Timer::getInstance();
    }
};

#endif /* SERVICE_LOCATOR_H_ */