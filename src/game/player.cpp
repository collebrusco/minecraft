#include "player.h"
#include "components.h"
#include <flgl.h>
LOG_MODULE(plyr);
using namespace glm;


Player Player::spawn(World &world, pos_t pos) {
    entID me = world.newEntity();
    world.addComp<c_Transform>(me, pos);
    world.addComp<c_LineOfSight>(me);
    world.addComp<c_Physics>(me);
    world.addComp<c_Cylinder>(me, c_Cylinder{0.4, 1.72, .1});
    world.addComp<c_Actor>(me).emplace<PlayerActor>();
    return Player{me};
}

#define sens (glm::vec2(0.002))
void PlayerActor::take_turn(const entID self, State& state) {

    c_LineOfSight& los = state.getComp<c_LineOfSight>(self);
    if (state.player_look && (window.mouse.delta.x != 0 || window.mouse.delta.y != 0)) {
        los.phi += window.mouse.delta.y * sens.y;
        los.theta -= window.mouse.delta.x * sens.x;
        los.updateLU();
    }

    vec3 move = vec3(0.);
    vec3 fwd = state.camera.readLook(); fwd.y = 0;
    vec3 rht = glm::cross(state.camera.readLook(), state.camera.readUp());
    float speed = 4.;
    if (window.keyboard[GLFW_KEY_PERIOD].down) {
        speed *= 5.;
    }
    if (window.keyboard[GLFW_KEY_COMMA].down) {
        speed *= 5.;
    }
    if (window.keyboard[GLFW_KEY_W].down) {
        move += fwd;
    }
    if (window.keyboard[GLFW_KEY_A].down) {
        move -= rht;
    }
    if (window.keyboard[GLFW_KEY_S].down) {
        move -= fwd;
    }
    if (window.keyboard[GLFW_KEY_D].down) {
        move += rht;
    }
    if (length(move) > 0.) {
        move = normalize(move);
    }
    move *= speed;
    if (state.creative) {
        if (window.keyboard[GLFW_KEY_SPACE].down) {
            move += (V3_UP * speed);        
        }
        if (window.keyboard[GLFW_KEY_LEFT_SHIFT].down) {
            move -= (V3_UP * speed);        
        }
    } else {
        if (window.keyboard[GLFW_KEY_SPACE].pressed) {
            if (state.getComp<c_Cylinder>(self).grounded(state, self))
                state.getComp<c_Physics>(self).velo.y = 9.f;
        }
    }
    /** move */
    state.addOrGetComp<c_Physics>(self).ofs = move;

    if (window.mouse.left().pressed && los.cast.hit()) {
        blockID* b = state.blockAt(los.cast.bpos);
        *b = *Blocks::AIR;
    }

    if (window.mouse.right().pressed && los.cast.hit()) {
        blockID* place = state.blockAt(los.cast.bpos + IDIRECTIONS[los.cast.face]);
        *place = *Blocks::STONE;
    }
    
}
