#version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main() {
    vec3 pos = aPos + vec3(0.5);
    gl_Position = uProj * uView * uModel * vec4(pos, 1.0f);
}
