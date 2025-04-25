#version 410 core
layout (location = 0) in vec2 aPos;

void main() {
    gl_PointSize = 8.;
    gl_Position = vec4(aPos, 0., 1.);
}
