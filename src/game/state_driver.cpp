#include "state_driver.h"
#include "components.h"
#include "actor.h"
#include "player.h"
#include "mob.h"
using namespace glm;
LOG_MODULE(sdriver);

void Physics::execute(State& state, const float dt) {
    State::Archetype<c_Transform, c_Physics> a;
    for (entID e : state.view(a)) {
        auto& phys = a.get<c_Physics>();
        const vec_t A = (phys.mass == 0.f || phys.velo.y < -TERMINAL_VELOCITY) ? vec_t(0.f) : vec_t(0.f, -GRAVITY, 0.f);
        const vec_t V = phys.ofs + phys.velo;
        phys.ofs = vec_t(0.f);
        phys.velo += (A * dt);

        const vec_t move = (V * dt) + (0.5f * A * dt * dt);
        pos_t newpos = a.get<c_Transform>().pos + move;

        c_Cylinder* cy = state.tryGetComp<c_Cylinder>(e);
        if (cy && state.cyl_collide(newpos, cy->rad, cy->height, cy->pivot)) {
            const pos_t oldpos = a.get<c_Transform>().pos;
            newpos = oldpos;

            newpos.x += move.x;
            if (state.cyl_collide(newpos, cy->rad, cy->height, cy->pivot)) {
                newpos.x = oldpos.x;
                a.get<c_Physics>().velo.x = 0.f;
            }
            newpos.z += move.z;
            if (state.cyl_collide(newpos, cy->rad, cy->height, cy->pivot)) {
                newpos.z = oldpos.z;
                a.get<c_Physics>().velo.z = 0.f;
            }
            newpos.y += move.y;
            if (state.cyl_collide(newpos, cy->rad, cy->height, cy->pivot)) {
                newpos.y = oldpos.y;
                a.get<c_Physics>().velo.y = 0.f;
            }
        }

        a.get<c_Transform>().pos = newpos;
    }
}

void StateDriver::init(State& state) {
    window.grab_mouse();
    state.camera.getFar() = 700.f;
    state.camera.getNear() = 0.01f;
    state.camera.update();

    state.player = Player::spawn(state, {14.f,100.f,14.f}).id;
    (void)Mob::spawn(state, {14.f, 100.f, 10.f});

    camcontrol.set_target(state.player);
}

void StateDriver::destroy() {
}

void StateDriver::tickA(State &state, const float dt) {
    action.execute(state);
    Physics::execute(state, dt);
}

void StateDriver::tickB(State &state, const float dt) {
    camcontrol.execute(state);
}

void StateDriver::tickC(State &state, const float dt) {
    c_LineOfSight::cast_all(state);
}

void ActionSystem::execute(State &state) {
    for (entID e : state.view<c_Actor>()) {
        Actor& actor = state.getComp<c_Actor>(e).get();
        actor.take_turn(e, state);
    }
}

void CameraControlSystem::execute(State& state) {
    if (state.entityValid(target)) {
        c_Transform* tf = state.tryGetComp<c_Transform>(target);
        c_LineOfSight* los = state.tryGetComp<c_LineOfSight>(target);
        if (tf)
            state.camera.getPos() = tf->pos;
        if (los) {
            state.camera.getLook() = los->look;
            state.camera.getUp() = los->up;
        }
    }
    
    cpos_t cpos = pos_to_cpos(state.camera.readPos());
    if (cpos != state.center()) {
        state.shift_to(cpos);
    }
    state.load_batch(1);

    state.camera.update();
}

void CameraControlSystem::set_target(entID e) {
    target = e;
}

entID CameraControlSystem::get_target() const {
    return target;
}
