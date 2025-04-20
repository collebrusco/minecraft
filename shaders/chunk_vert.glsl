#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNorm;

layout (location = 3) in uint aInstance;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec2 iUV;
out vec3 iPos;
out vec3 iNorm;

#define V3_UP      (vec3( 0.f, 1.f, 0.f))
#define V3_DOWN    (vec3( 0.f,-1.f, 0.f))
#define V3_NORTH   (vec3( 0.f, 0.f, 1.f))
#define V3_SOUTH   (vec3( 0.f, 0.f,-1.f))
#define V3_EAST    (vec3( 1.f, 0.f, 0.f))
#define V3_WEST    (vec3(-1.f, 0.f, 0.f))

#define V2_NORTH   (vec2( 0.f,  1.f))
#define V2_SOUTH   (vec2( 0.f, -1.f))
#define V2_EAST    (vec2( 1.f,  0.f))
#define V2_WEST    (vec2(-1.f,  0.f))

#define TOP      (0)
#define BOT      (1)
#define NORTH    (2)
#define SOUTH    (3)
#define EAST     (4)
#define WEST     (5)

struct Instance {
    uint orientation;
    float x, y, z;
    uint tex;
};

vec3 rotate_by_orient(vec3 v, uint o) {
    vec3 res = v;

    res = mix(res, vec3( v.x, -v.z,  v.y), float(o == TOP));
    res = mix(res, vec3( v.x,  v.z, -v.y), float(o == BOT));
    res = mix(res, vec3(-v.x,  v.y, -v.z), float(o == NORTH));
    res = mix(res, vec3(-v.z,  v.y,  v.x), float(o == EAST));
    res = mix(res, vec3( v.z,  v.y, -v.x), float(o == WEST));

    return res;
}

vec3 normal_from_orient(uint o) {
    vec3 res = vec3(0.);

    res = mix(res, V3_UP, float(o == TOP));
    res = mix(res, V3_DOWN, float(o == BOT));
    res = mix(res, V3_NORTH, float(o == NORTH));
    res = mix(res, V3_EAST, float(o == EAST));
    res = mix(res, V3_WEST, float(o == WEST));
    res = mix(res, V3_SOUTH, float(o == SOUTH));

    return res;
}


void main() {

    Instance instance;
    instance.orientation = (aInstance >> 16u) & 0x7u;
    instance.x = float(aInstance & 0xFu);
    instance.z = float((aInstance >> 4) & 0xFu);
    instance.y = float((aInstance >> 8) & 0xFFu);


    vec3 rPos = rotate_by_orient(aPos, instance.orientation) + vec3(instance.x, instance.y, instance.z);
    vec3 rNorm = normal_from_orient(instance.orientation);

    vec3 worldPos = rPos;

    iUV = aUV;
    iPos = worldPos;
    iNorm = rNorm;
    gl_Position = uProj * uView * uModel * vec4(worldPos, 1.0f);
}
