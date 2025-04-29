#version 410 core
out vec4 outColor;

in  vec3 iNorm;
in  vec2 iUV; 
flat in uint texi;

uniform sampler2D uTex;
#define SHEET_SIZE      (64)
#define SHEET_SIZE_F    (64.)
#define FACE_PIX        (16)
#define FACE_PIX_F      (16.)
#define SHEET_SIZE_PIX      (FACE_PIX * SHEET_SIZE)
#define SHEET_SIZE_PIX_F    (FACE_PIX_F * SHEET_SIZE_F)
#define FACE_SIZE_UV    (1. / SHEET_SIZE_F)

vec2 texi_to_botleft(uint texi) {
    return vec2(texi % SHEET_SIZE, texi / SHEET_SIZE) * FACE_SIZE_UV;
}

void main(){
    vec2 UV = iUV;
    vec4 base = texture(uTexi, texi_to_botleft(texi), + (UV * FACE_SIZE_UV));
    outColor = base;
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
