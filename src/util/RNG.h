/** 
 * RNG.h 
 * minecraft
 * created 04/20/25 by frank collebrusco
 */
#ifndef RNG_H
#define RNG_H
#include "standard.h"

struct RNG {
    static uint32_t seed;

    static void setSeed(uint32_t s);
    static float lerp(float a, float b, float t);
    static float smootherstep(float a, float b, float t);
    static uint32_t hash(int32_t x, int32_t y);

    static float random_scalar_int(int32_t x, int32_t y);
    static float random_scalar(float x, float y);
    static float random_scalar_octave(float x, float y, int oct);
    static glm::vec2 random_unit_vec(int32_t x, int32_t y);
};



#endif /* RNG_H */
