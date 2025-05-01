/** 
 * NOISE.h 
 * minecraft
 * created 04/20/25 by frank collebrusco
 */
#ifndef NOISE_H
#define NOISE_H
#include "standard.h"

struct Noise {
    uint64_t seed;
    explicit Noise(uint64_t seed = 0x4200FEED19982005ull);

    void setSeed(uint64_t s);

    static constexpr float lerp(float a, float b, float t);
    static constexpr float smootherstep(float a, float b, float t);

    inline static uint32_t hash(int32_t x, int32_t y, uint64_t seed);
    inline static uint32_t hash(int32_t x, int32_t y, int32_t z, uint64_t seed);

    inline uint32_t hash(int32_t x, int32_t y) const;
    inline uint32_t hash(int32_t x, int32_t y, int32_t z) const;

    inline float valueAt(int32_t x, int32_t y) const;
    inline float valueAt(int32_t x, int32_t y, int32_t z) const;

    float value(float x, float y) const;
    float value(float x, float y, float z) const;

    float value_octave(float x, float y, int octaves) const;

    glm::vec2 unitVec(int32_t x, int32_t y) const;
    glm::vec3 unitVec(int32_t x, int32_t y, int32_t z) const;
};

struct Perlin2 : private Noise {
    Perlin2(uint64_t s = 0x4200FEED19982005ull);
    float perlin(glm::vec2 pos, float rotation = 0.f, glm::vec2 scale = glm::vec2(1.)) const;
};

struct Perlin3 : private Noise {
    Perlin3(uint64_t s = 0x4200FEED19982005ull);
    float perlin(glm::vec3 pos, glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.)) const;
};

#endif /* NOISE_H */
