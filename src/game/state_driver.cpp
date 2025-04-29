#include "state_driver.h"
#include "components.h"

void Physics::execute(State& state) {
    State::Archetype<c_Transform, c_Physics> a;
    for (entID e : state.view(a)) {
        pos_t newpos = a.get<c_Transform>().pos + a.get<c_Physics>().ofs;

        c_Cylinder* cy = state.tryGetComp<c_Cylinder>(e);
        if (cy && state.cyl_collide(newpos, cy->rad, cy->height, cy->pivot)) {
            const pos_t oldpos = a.get<c_Transform>().pos;
            const vec_t ofs = a.get<c_Physics>().ofs;
            newpos = oldpos;

            newpos.x += ofs.x;
            if (state.cyl_collide(newpos, cy->rad, cy->height, cy->pivot)) {
                newpos.x = oldpos.x;
            }
            newpos.z += ofs.z;
            if (state.cyl_collide(newpos, cy->rad, cy->height, cy->pivot)) {
                newpos.z = oldpos.z;
            }
            newpos.y += ofs.y;
            if (state.cyl_collide(newpos, cy->rad, cy->height, cy->pivot)) {
                newpos.y = oldpos.y;
            }
        }

        a.get<c_Transform>().pos = newpos;
        a.get<c_Physics>().ofs = vec_t(0.f);
    }
}

void StateDriver::init() {
}

void StateDriver::destroy() {
}

void StateDriver::tick(State &state, const float dt) {
    Physics::execute(state);
}

void ActionSystem::execute(State &state, const float dt) {
}
