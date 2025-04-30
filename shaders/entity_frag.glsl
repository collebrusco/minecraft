#version 410 core
out vec4 outColor;

in  vec3 iNorm;
in  vec2 iUV; 

uniform sampler2D uTex;


void main(){
    vec2 UV = iUV; UV.y = 1. - UV.y;
    outColor = texture(uTex, UV);
    //outColor = vec4(iUV, 0., 1.);
    //vec3 c = iNorm;
    //if (c.r < -0.1) {
    //    c.r = 1.; c.b = 1.;
    //}
    //if (c.g < -0.1) {
    //    c.g = 1.; c.r = 1.;
    //}
    //if (c.b < -0.1) {
    //    c.b = 1.; c.y = 1.;
    //}
    //outColor = vec4(c, 1.);
    //outColor = vec4(1.);
}
