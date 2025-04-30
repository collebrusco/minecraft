#include "Noise.h"
using namespace glm;
LOG_MODULE(noise);

Noise::Noise(uint64_t sd) : seed(sd) {}

void Noise::setSeed(uint64_t s) {
    seed = s;
}

constexpr float Noise::lerp(float a, float b, float t) {
    return a * (1.0f - t) + b * t;
}

constexpr float Noise::smootherstep(float a, float b, float t) {
    t = glm::clamp(t, 0.0f, 1.0f);
    float smoothT = t * t * t * (t * (t * 6 - 15) + 10);
    return a * (1.0f - smoothT) + b * smoothT;
}

inline uint32_t Noise::hash(int32_t x, int32_t y, uint64_t seed) {
    uint32_t h = static_cast<uint32_t>(x) * 374761393u
               + static_cast<uint32_t>(y) * 668265263u
               + static_cast<uint32_t>(seed * 1443059017u);
    h ^= (h >> 13);
    return h * 1274126177u;
}

inline uint32_t Noise::hash(int32_t x, int32_t y, int32_t z, uint64_t seed) {
    uint32_t h = static_cast<uint32_t>(x) * 374761393u
               + static_cast<uint32_t>(y) * 668265263u
               + static_cast<uint32_t>(z) * 2147483647u
               + static_cast<uint32_t>(seed * 1443059017u);
    h ^= (h >> 13);
    return h * 1274126177u;
}

inline uint32_t Noise::hash(int32_t x, int32_t y) const {
    return hash(x, y, seed);
}

inline uint32_t Noise::hash(int32_t x, int32_t y, int32_t z) const {
    return hash(x, y, z, seed);
}

inline float Noise::valueAt(int32_t x, int32_t y) const {
    return float(hash(x, y)) / 4294967296.0f;
}

inline float Noise::valueAt(int32_t x, int32_t y, int32_t z) const {
    return float(hash(x, y, z)) / 4294967296.0f;
}

float Noise::value(float x, float y) const {
    int32_t x0 = static_cast<int32_t>(glm::floor(x));
    int32_t y0 = static_cast<int32_t>(glm::floor(y));
    int32_t x1 = x0 + 1;
    int32_t y1 = y0 + 1;

    float fx = x - float(x0);
    float fy = y - float(y0);

    float v00 = valueAt(x0, y0);
    float v10 = valueAt(x1, y0);
    float v01 = valueAt(x0, y1);
    float v11 = valueAt(x1, y1);

    float i1 = smootherstep(v00, v10, fx);
    float i2 = smootherstep(v01, v11, fx);
    return smootherstep(i1, i2, fy);
}

float Noise::value(float x, float y, float z) const {
    int32_t x0 = static_cast<int32_t>(glm::floor(x));
    int32_t iy0 = static_cast<int32_t>(glm::floor(y));
    int32_t z0 = static_cast<int32_t>(glm::floor(z));
    int32_t x1 = x0 + 1;
    int32_t iy1 = iy0 + 1;
    int32_t z1 = z0 + 1;

    float fx = x - float(x0);
    float fy = y - float(iy0);
    float fz = z - float(z0);

    float v000 = valueAt(x0, iy0, z0);
    float v100 = valueAt(x1, iy0, z0);
    float v010 = valueAt(x0, iy1, z0);
    float v110 = valueAt(x1, iy1, z0);
    float v001 = valueAt(x0, iy0, z1);
    float v101 = valueAt(x1, iy0, z1);
    float v011 = valueAt(x0, iy1, z1);
    float v111 = valueAt(x1, iy1, z1);

    float x00 = smootherstep(v000, v100, fx);
    float x10 = smootherstep(v010, v110, fx);
    float x01 = smootherstep(v001, v101, fx);
    float x11 = smootherstep(v011, v111, fx);

    float y0 = smootherstep(x00, x10, fy);
    float y1 = smootherstep(x01, x11, fy);

    return smootherstep(y0, y1, fz);
}

float Noise::value_octave(float x, float y, int oct) const {
    float noise = 0;
    noise = noise + (value((x / 256.), (y / 256.)));
    for (int i = 1; i <= oct; i++) {
        const float scale = (1 << (oct - i));
        const float offsetX = (valueAt(i * 1297, i * 7919) - 0.5) * scale * 2;
        const float offsetY = (valueAt(i * 3571, i * 15731) - 0.5) * scale * 2;
        noise = noise + (value((x / scale) + offsetX, (y / scale) + offsetY) / ((float)(i * i) / 2.f));
    }
    return noise;
}

glm::vec2 Noise::unitVec(int32_t x, int32_t y) const {
    uint32_t h = hash(x, y);
    float angle = (float(h) / 4294967296.0f) * glm::two_pi<float>();
    return glm::vec2(glm::cos(angle), glm::sin(angle));
}

glm::vec3 Noise::unitVec(int32_t x, int32_t y, int32_t z) const {
    uint32_t h = hash(x, y, z);
    float theta = ((h & 0xFFFF) / 65536.0f) * glm::two_pi<float>();
    float phi = (((h >> 16) & 0xFFFF) / 65536.0f) * glm::pi<float>();

    float x_ = glm::sin(phi) * glm::cos(theta);
    float y_ = glm::sin(phi) * glm::sin(theta);
    float z_ = glm::cos(phi);
    return glm::vec3(x_, y_, z_);
}

static glm::vec2 rotate2D(glm::vec2 v, float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return glm::vec2(c * v.x - s * v.y, s * v.x + c * v.y);
}

static glm::vec3 rotate3D(glm::vec3 v, glm::vec3 rot) {
    glm::mat4 M(1.0f);
    M = glm::rotate(M, rot.x, glm::vec3(1, 0, 0));
    M = glm::rotate(M, rot.y, glm::vec3(0, 1, 0));
    M = glm::rotate(M, rot.z, glm::vec3(0, 0, 1));
    return glm::vec3(M * glm::vec4(v, 0.0f));
}

Perlin2::Perlin2(uint64_t s) : Noise(s) {}
float Perlin2::perlin(glm::vec2 pos, float rotation, glm::vec2 scale) {
    pos *= scale;

    glm::ivec2 p0 = glm::floor(pos);
    glm::ivec2 p1 = p0 + glm::ivec2(1);
    glm::vec2 f = pos - glm::vec2(p0);

    float u = f.x;
    float v = f.y;

    glm::vec2 g00 = rotate2D(unitVec(p0.x, p0.y), rotation);
    glm::vec2 g10 = rotate2D(unitVec(p1.x, p0.y), rotation);
    glm::vec2 g01 = rotate2D(unitVec(p0.x, p1.y), rotation);
    glm::vec2 g11 = rotate2D(unitVec(p1.x, p1.y), rotation);

    glm::vec2 d00 = f - glm::vec2(0, 0);
    glm::vec2 d10 = f - glm::vec2(1, 0);
    glm::vec2 d01 = f - glm::vec2(0, 1);
    glm::vec2 d11 = f - glm::vec2(1, 1);

    float i00 = glm::dot(g00, d00);
    float i10 = glm::dot(g10, d10);
    float i01 = glm::dot(g01, d01);
    float i11 = glm::dot(g11, d11);

    float ix0 = smootherstep(i00, i10, u);
    float ix1 = smootherstep(i01, i11, u);
    return smootherstep(ix0, ix1, v);
}

float Perlin3::perlin(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale) {
    pos *= scale;

    glm::ivec3 p0 = glm::floor(pos);
    glm::ivec3 p1 = p0 + glm::ivec3(1);
    glm::vec3 f = pos - glm::vec3(p0);

    float u = f.x;
    float v = f.y;
    float w = f.z;

    glm::vec3 g000 = rotate3D(unitVec(p0.x, p0.y, p0.z), rotation);
    glm::vec3 g100 = rotate3D(unitVec(p1.x, p0.y, p0.z), rotation);
    glm::vec3 g010 = rotate3D(unitVec(p0.x, p1.y, p0.z), rotation);
    glm::vec3 g110 = rotate3D(unitVec(p1.x, p1.y, p0.z), rotation);
    glm::vec3 g001 = rotate3D(unitVec(p0.x, p0.y, p1.z), rotation);
    glm::vec3 g101 = rotate3D(unitVec(p1.x, p0.y, p1.z), rotation);
    glm::vec3 g011 = rotate3D(unitVec(p0.x, p1.y, p1.z), rotation);
    glm::vec3 g111 = rotate3D(unitVec(p1.x, p1.y, p1.z), rotation);

    glm::vec3 d000 = f - glm::vec3(0, 0, 0);
    glm::vec3 d100 = f - glm::vec3(1, 0, 0);
    glm::vec3 d010 = f - glm::vec3(0, 1, 0);
    glm::vec3 d110 = f - glm::vec3(1, 1, 0);
    glm::vec3 d001 = f - glm::vec3(0, 0, 1);
    glm::vec3 d101 = f - glm::vec3(1, 0, 1);
    glm::vec3 d011 = f - glm::vec3(0, 1, 1);
    glm::vec3 d111 = f - glm::vec3(1, 1, 1);

    float i000 = glm::dot(g000, d000);
    float i100 = glm::dot(g100, d100);
    float i010 = glm::dot(g010, d010);
    float i110 = glm::dot(g110, d110);
    float i001 = glm::dot(g001, d001);
    float i101 = glm::dot(g101, d101);
    float i011 = glm::dot(g011, d011);
    float i111 = glm::dot(g111, d111);

    float x00 = smootherstep(i000, i100, u);
    float x10 = smootherstep(i010, i110, u);
    float x01 = smootherstep(i001, i101, u);
    float x11 = smootherstep(i011, i111, u);

    float y0 = smootherstep(x00, x10, v);
    float y1 = smootherstep(x01, x11, v);

    return smootherstep(y0, y1, w);
}


