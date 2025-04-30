/** 
 * player.h 
 * minecraft
 * created 04/27/25 by frank collebrusco
 */
#ifndef PLAYER_H
#define PLAYER_H
#include "standard.h"
#include "actor.h"
#include "data/World.h"

struct PlayerActor : public Actor {
    virtual void take_turn(const entID self, State& state) override final;
};

struct Player {
    entID id;
    static Player spawn(World& world, pos_t pos);
};


#endif /* PLAYER_H */
