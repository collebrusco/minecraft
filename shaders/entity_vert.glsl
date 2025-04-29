#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aNorm;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uModelPartMat;
uniform vec2 uUVMin[6];
uniform vec2 uUVMax[6];

out vec3 iNorm;
out vec2 iUV;


#define V3_UP      (vec3( 0.f, 1.f, 0.f))
#define V3_DOWN    (vec3( 0.f,-1.f, 0.f))
#define V3_NORTH   (vec3( 0.f, 0.f, 1.f))
#define V3_SOUTH   (vec3( 0.f, 0.f,-1.f))
#define V3_EAST    (vec3( 1.f, 0.f, 0.f))
#define V3_WEST    (vec3(-1.f, 0.f, 0.f))

#define TOP      (0)
#define BOT      (1)
#define NORTH    (2)
#define SOUTH    (3)
#define EAST     (4)
#define WEST     (5)

uint orient_from_normal(vec3 norm) {
    if (norm == V3_UP) return TOP;
    if (norm == V3_DOWN) return BOT;
    if (norm == V3_NORTH) return NORTH;
    if (norm == V3_SOUTH) return SOUTH;
    if (norm == V3_EAST) return EAST;
    if (norm == V3_WEST) return WEST;
    return 0u; // default case (shouldn't happen ideally)
}
void main() {
    // determine what face you are on 
    iNorm = aNorm;
    uint face = orient_from_normal(aNorm);
    iUV = mix(uUVMin[face], uUVMax[face], aUv);
    gl_Position = uProj * uView * uModel * uModelPartMat * vec4(aPos, 1.0f); 
}
