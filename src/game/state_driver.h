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
    static void execute(State& state, const float dt);
};

struct ActionSystem {
    void execute(State& state);
};

struct CameraControlSystem {
    CameraControlSystem() : target(0xFFFFFFFFFFFFFFFF) {}
    void execute(State& state);
    void set_target(entID e);
    entID get_target() const;
private:
    entID target;
};

struct StateDriver {
    void init(State& state);
    void destroy();

    void tickA(State& state, const float dt);
    void tickB(State& state, const float dt);
    void tickC(State& state, const float dt);

private:
    ActionSystem action;
    CameraControlSystem camcontrol;
};


#endif /* STATE_DRIVER_H */
