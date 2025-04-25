#version 410 core
in  vec3 iNorm;
out vec4 outColor;

void main(){
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
    //outColor = vec4(1.);
}
