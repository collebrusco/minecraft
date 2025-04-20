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

void main() {
    iUV = aUV;
    iPos = aPos;
    iNorm = aNorm;
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0f);
}
