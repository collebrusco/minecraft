/** 
 * state.h 
 * minecraft
 * created 04/28/25 by frank collebrusco
 */
#ifndef STATE_H
#define STATE_H
#include "standard.h"
#include "World.h"

struct State : public World {
    State(WorldGenerator& wgen);
    virtual ~State() = default;

    MousePerspectiveCamera camera;

    entID player;

    State::RaycastResult cast;
};

#endif /* STATE_H */
