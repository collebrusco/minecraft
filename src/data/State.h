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

    PerspectiveCamera camera;

    void set_creative(bool c = true);
    void set_playerlook(bool p = true);

    bool creative;
    bool player_look;
    entID player;
};

#endif /* STATE_H */
