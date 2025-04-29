/** 
 * components.h 
 * minecraft
 * created 04/27/25 by frank collebrusco
 */
#ifndef COMPONENTS_H
#define COMPONENTS_H
#include "standard.h"

struct c_Transform {
    c_Transform(vec_t p = {0.f,0.f,0.f}, vec_t r = {0.f,0.f,0.f}, vec_t s = {1.f,1.f,1.f}, vec_t a = {0.f,0.f,0.f})
    : pos(p), rot(r), scale(s), anc(a) {}
    vec_t pos, rot, scale, anc;
};

struct c_Physics {
    vec_t ofs;
    vec_t velo;
};

struct c_Cylinder {
    float rad, height, pivot;
};

struct c_LineOfSight {
    vec_t look, up;
};

#endif /* COMPONENTS_H */
