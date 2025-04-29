#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aNorm;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uModelPart;
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

void main() {
    iNorm = aNorm;
    // determine what face you are on 
    gl_Position = uProj * uView * uModel * uModelPart * vec4(aPos, 1.0f); 
    iUV = aUV;
    //gl_Position = vec4(aPos, 1.0f);
}
