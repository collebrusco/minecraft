#include "RNG.h"

uint32_t RNG::seed = 0;

void RNG::setSeed(uint32_t s) {
    RNG::seed = s;
}

float RNG::lerp(float a, float b, float t) {
    return a * (1 - t) + b * t;
}

float RNG::smootherstep(float a, float b, float t) {
    t = glm::max(0.f, glm::min(1.f, t));
    float smoothT = t * t * t * (t * (t * 6 - 15) + 10);
    return a * (1 - smoothT) + b * smoothT;
}

uint32_t RNG::hash(int32_t x, int32_t y) {
    uint32_t h = x * 374761393 + y * 668265263 + RNG::seed * 1443059017;
    h = (h ^ (h >> 13));
    h = (h * 1274126177);
    return h;
}

float RNG::random_scalar_int(int32_t x, int32_t y) {
    return float(RNG::hash(x, y)) * (1.0f / 4294967296.0f);
}

float RNG::random_scalar(float x, float y) {
    const int32_t x0 = glm::floor(x);
    const int32_t y0 = glm::floor(y);
    const int32_t x1 = x0 + 1;
    const int32_t y1 = y0 + 1;

    const double sx = x - x0;
    const double sy = y - y0;

    const float v00 = RNG::random_scalar_int(x0, y0);
    const float v10 = RNG::random_scalar_int(x1, y0);
    const float v01 = RNG::random_scalar_int(x0, y1);
    const float v11 = RNG::random_scalar_int(x1, y1);

    const float i1 = RNG::smootherstep(v00, v10, sx);
    const float i2 = RNG::smootherstep(v01, v11, sx);
    return RNG::smootherstep(i1, i2, sy);
}

float RNG::random_scalar_octave(float x, float y, int oct) {
    float noise = 0;
    noise = noise + (RNG::random_scalar((x / 256.), (y / 256.)));
    for (int i = 1; i <= oct; i++) {
        const float scale = (1 << (oct - i));
        const float offsetX = (RNG::random_scalar_int(i * 1297, i * 7919) - 0.5) * scale * 2;
        const float offsetY = (RNG::random_scalar_int(i * 3571, i * 15731) - 0.5) * scale * 2;
        noise = noise + (RNG::random_scalar((x / scale) + offsetX, (y / scale) + offsetY) / ((float)(i * i) / 2.f));
    }
    return noise;
}

glm::vec2 RNG::random_unit_vec(int32_t x, int32_t y) {
    const int32_t h = RNG::hash(x, y);
    const float angle = (((double)h) / ((double)0xFFFFFFFF)) * 2.0 * 3.14159265;
    return glm::vec2{glm::cos(angle), glm::sin(angle)};
}


