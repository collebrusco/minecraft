#include "player.h"
#include "components.h"
#include <flgl.h>
LOG_MODULE(plyr);
using namespace glm;

void PlayerActor::take_turn(const entID self, State& state) {
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
    if (window.keyboard[GLFW_KEY_L].pressed) {
        static bool mg = true;
        mg = !mg;
        window.set_mouse_grab(mg);
    }
    if (length(move) > 0.) {
        move = normalize(move);
    }
    move *= speed;
    if (window.keyboard[GLFW_KEY_SPACE].down) {
        move += (V3_UP * speed);
    }
    if (window.keyboard[GLFW_KEY_LEFT_SHIFT].down) {
        move -= (V3_UP * speed);        
    }
    /** move */
    state.addOrGetComp<c_Physics>(self).ofs = move;

    if (window.mouse.left().pressed && state.cast.hit()) {
        Block* b = state.blockAt(state.cast.bpos);
        *b = Block::null();
    }

    if (window.mouse.right().pressed && state.cast.hit()) {
        Block* place = state.blockAt(state.cast.bpos + IDIRECTIONS[state.cast.face]);
        place->type = &Blocks::stone;
    }
    
}

Player Player::spawn(World &world, pos_t pos) {
    entID me = world.newEntity();
    world.addComp<c_Transform>(me, pos);
    world.addComp<c_Physics>(me, c_Physics{vec_t(0.f), vec_t(0.f)});
    world.addComp<c_Cylinder>(me, c_Cylinder{0.4, 1.72, .1});
    world.addComp<c_Actor>(me).emplace<PlayerActor>();
    return Player{me};
}
