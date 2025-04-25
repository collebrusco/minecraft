#version 410 core
layout (location = 0) in vec3 aPos;

uniform vec3 uCampos;
uniform mat4 uView;
uniform mat4 uProj;

void main() {
    vec4 pos = uProj * uView * vec4(aPos, 1.);
    // pos = pos + (0.1f * vec4(normalize(uCampos - pos.xyz), 0.));
    // pos = uProj * uView * pos;
    gl_Position = pos;
    gl_PointSize = 12.;
}
