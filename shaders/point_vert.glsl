#version 410 core
layout (location = 0) in vec3 aPos;

uniform vec3 uCampos;
uniform mat4 uView;
uniform mat4 uProj;

void main() {
    vec3 pos = aPos + (0.1f * normalize(uCampos - aPos));
    gl_Position = uProj * uView * vec4(pos, 1.);
    gl_PointSize = 12.;
}
