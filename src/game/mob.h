#ifndef MOB_H
#define MOB_H
#include "standard.h"
#include "actor.h"
#include "data/World.h"

struct CreeperAI : public Actor {
    virtual void take_turn(const entID self, State& state) override final;
};

struct Mob {
    entID id;
    static Mob spawn(World& world, pos_t pos);
};

#endif /* MOB_H */
