/** 
 * state_driver.h 
 * minecraft
 * created 04/28/25 by frank collebrusco
 */
#ifndef STATE_DRIVER_H
#define STATE_DRIVER_H
#include "standard.h"
#include "data/State.h"

struct Physics {
    static void execute(State& state);
};

struct ActionSystem {
    void execute(State& state, const float dt);
private:
    
};

struct StateDriver {
    void init();
    void destroy();

    void tick(State& state, const float dt);

private:
    
};


#endif /* STATE_DRIVER_H */
