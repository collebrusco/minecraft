/** 
 * components.h 
 * minecraft
 * created 04/27/25 by frank collebrusco
 */
#ifndef COMPONENTS_H
#define COMPONENTS_H
#include "standard.h"
#include "data/World.h"

struct c_Transform {
    c_Transform(vec_t p = {0.f,0.f,0.f}, vec_t r = {0.f,0.f,0.f}, vec_t s = {1.f,1.f,1.f}, vec_t a = {0.f,0.f,0.f})
    : pos(p), rot(r), scale(s), anc(a) {}
    vec_t pos, rot, scale, anc;
};

struct c_Physics {
    c_Physics() : ofs(), velo(), mass(1.f) {}
    vec_t ofs;
    vec_t velo;
    float mass;
};

struct c_Cylinder {
    float rad, height, pivot;
    inline bool grounded(World& world, entID me) const {
        return world.cyl_collide(world.getComp<c_Transform>(me).pos + pos_t{0.f,-0.05f,0.f}, rad, height, pivot);
    }
};

struct c_LineOfSight {
    c_LineOfSight() : phi(3.14159265*0.5), theta(0), look(0.f,0.f,-1.f), up(0.f,1.f,0.f) {}
    float phi, theta;
    vec_t look, up;
    World::RaycastResult cast;
    inline void updateLU() {
        vec_t anchor = glm::vec3(-glm::cos(theta), 0.0, glm::sin(theta));
        look = glm::vec3(-1 * glm::sin(phi) * glm::sin(theta),
                    glm::cos(phi),
                    -1 * glm::sin(phi) * glm::cos(theta));
        up = cross(look, anchor);
    }
    inline void raycast(World& world, entID me) {
        cast = world.raycast(Ray(world.getComp<c_Transform>(me).pos, look));
    }
    inline static void cast_all(World& world) {
        for (entID e : world.view<c_Transform, c_LineOfSight>()) {
            world.getComp<c_LineOfSight>(e).raycast(world, e);
        }
    }
};

struct c_Mob {}; // tag only, no data

#endif /* COMPONENTS_H */
