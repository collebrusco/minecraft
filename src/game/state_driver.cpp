#include "state_driver.h"
#include "components.h"
#include "actor.h"
#include "player.h"
using namespace glm;
LOG_MODULE(sdriver);

void Physics::execute(State& state, const float dt) {
    State::Archetype<c_Transform, c_Physics> a;
    for (entID e : state.view(a)) {
        {
            auto& phys = a.get<c_Physics>();
            phys.velo.y -= (GRAVITY * dt);
            phys.ofs += (phys.velo);
        }
        {
            const vec_t ofs = a.get<c_Physics>().ofs * dt;
            pos_t newpos = a.get<c_Transform>().pos + ofs;

            c_Cylinder* cy = state.tryGetComp<c_Cylinder>(e);
            if (cy && state.cyl_collide(newpos, cy->rad, cy->height, cy->pivot)) {
                const pos_t oldpos = a.get<c_Transform>().pos;
                newpos = oldpos;

                newpos.x += ofs.x;
                if (state.cyl_collide(newpos, cy->rad, cy->height, cy->pivot)) {
                    newpos.x = oldpos.x;
                    a.get<c_Physics>().velo.x = 0.f;
                }
                newpos.z += ofs.z;
                if (state.cyl_collide(newpos, cy->rad, cy->height, cy->pivot)) {
                    newpos.z = oldpos.z;
                    a.get<c_Physics>().velo.z = 0.f;
                }
                newpos.y += ofs.y;
                if (state.cyl_collide(newpos, cy->rad, cy->height, cy->pivot)) {
                    newpos.y = oldpos.y;
                    a.get<c_Physics>().velo.y = 0.f;
                }
            }

            a.get<c_Transform>().pos = newpos;
            a.get<c_Physics>().ofs = vec_t(0.f);
        }
    }
}

void StateDriver::init(State& state) {
    window.grab_mouse();
    state.camera.getFar() = 700.f;
    state.camera.getNear() = 0.01f;
    state.camera.update();

    state.player = Player::spawn(state, {14.f,16.f,14.f}).id;
    camcontrol.set_target(state.player);
}

void StateDriver::destroy() {
}

void StateDriver::tick(State &state, const float dt) {
    action.execute(state);
    Physics::execute(state, dt);
    camcontrol.execute(state);
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

    state.camera.update();
}

void CameraControlSystem::set_target(entID e) {
    target = e;
}

entID CameraControlSystem::get_target() const {
    return target;
}
