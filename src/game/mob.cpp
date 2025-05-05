#include "mob.h"
#include "components.h"
#include <flgl.h>
LOG_MODULE(mob);

using namespace glm;

Mob Mob::spawn(World& world, pos_t pos) {
    entID mob = world.newEntity();
    world.addComp<c_Transform>(mob, pos);
    world.addComp<c_LineOfSight>(mob);
    world.addComp<c_Physics>(mob);
    world.addComp<c_Cylinder>(mob, c_Cylinder{0.4, 1.72, 1.5});
    world.addComp<c_Actor>(mob).emplace<CreeperAI>();
    world.addComp<c_Mob>(mob);
    return Mob{mob};
}

void CreeperAI::take_turn(const entID self, State& state) {
    vec3 player_pos = state.getComp<c_Transform>(state.player).pos;
    c_Transform& tf = state.getComp<c_Transform>(self);
    c_Physics& phys = state.getComp<c_Physics>(self);

    vec3 direction = player_pos - tf.pos;
    direction.y = 0.f;
    float dist2 = glm::length2(direction);

    constexpr float max_dist = 20.0f;
    constexpr float exp_dist = 2.f;
    if (dist2 < exp_dist * exp_dist) {
        constexpr int exp_rad = 6;
        for (int i = 0; i < exp_rad; i++) {
            for (int j = 0; j < exp_rad; j++) {
                for (int k = 0; k < exp_rad; k++) {
                    bpos_t pos = pos_to_bpos(tf.pos) + bpos_t{-exp_rad/2,-exp_rad/2,-exp_rad/2} + bpos_t{i,j,k};
                    // LOG_INF("HERE %.2f < %.2f", glm::length(bpos_to_pos(pos) - tf.pos), (float)exp_rad);
                    // LOG_INF("%d,%d,%d", pos.x, pos.y, pos.z);
                    if ((glm::length(bpos_to_pos(pos) - tf.pos)) < (float)exp_rad/2){
                        blockID* tar = state.blockAt(pos);
                        if (tar) *tar = *Blocks::AIR;
                    }
                }
            }
        }
        entID me = self;
        state.removeEntity(me);
    } else if (dist2 < max_dist * max_dist) {
        vec3 move = normalize(direction);
        move.y = 0.0f;
        phys.ofs = move * 2.0f;
    } else {
        phys.ofs = vec3(0.0f);
    }
}
