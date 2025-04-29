/** 
 * actor.h 
 * minecraft
 * created 04/27/25 by frank collebrusco
 */
#ifndef ACTOR_H
#define ACTOR_H
#include "standard.h"
#include "data/State.h"

struct Actor {
    virtual ~Actor() = default;
    virtual void take_turn(const entID self, State& state) = 0;
};

struct c_Actor : public c_abstract<Actor, 128> {};

#endif /* ACTOR_H */
