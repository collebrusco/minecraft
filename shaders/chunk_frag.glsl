#version 410 core

out vec4 outColor;

in vec2 iUV;
in vec3 iPos;
in vec3 iNorm;

uniform vec3 uLightdir;

void main(){
    float kd = max(0.,dot(-uLightdir, iNorm));
    float ka = (kd / 20.) + 0.1;
    outColor = vec4(vec3(ka + kd),0.);
    vec3 c = iNorm;
    if (c.r < -0.1) {
        c.r = 1.; c.b = 1.;
    }
    if (c.g < -0.1) {
        c.g = 1.; c.r = 1.;
    }
    if (c.b < -0.1) {
        c.b = 1.; c.y = 1.;
    }
    outColor = vec4(c, 1.);
}
