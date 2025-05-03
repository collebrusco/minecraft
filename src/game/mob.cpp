#include "mob.h"
#include "components.h"
#include <flgl.h>
LOG_MODULE(mob);

using namespace glm;

Mob Mob::spawn(World& world, pos_t pos) {
    entID mob = world.newEntity();
    world.addComp<c_Transform>(mob, pos);
    world.addComp<c_LineOfSight>(mob);  // Not strictly needed now, but useful for vision-based AI later
    world.addComp<c_Physics>(mob);
    world.addComp<c_Cylinder>(mob, c_Cylinder{0.4, 1.72, 1.5});
    world.addComp<c_Actor>(mob).emplace<CreeperAI>();
    world.addComp<c_Mob>(mob);
    return Mob{mob};
}

void CreeperAI::take_turn(const entID self, State& state) {
    // Get player and mob positions
    vec3 player_pos = state.getComp<c_Transform>(state.player).pos;
    c_Transform& tf = state.getComp<c_Transform>(self);
    c_Physics& phys = state.getComp<c_Physics>(self);

    // Direction toward player
    vec3 direction = player_pos - tf.pos;
    float dist2 = glm::length2(direction);

    constexpr float max_dist = 12.0f; // follow range
    if (dist2 < max_dist * max_dist) {
        vec3 move = normalize(direction);
        move.y = 0.0f; // optional: disable vertical climbing
        phys.ofs = move * 2.0f; // creeper speed
    } else {
        phys.ofs = vec3(0.0f);
    }
}
