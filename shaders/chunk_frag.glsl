#version 410 core

out vec4 outColor;

in vec2 iUV;
in vec3 iPos;
in vec3 iNorm;
flat in uint texi;

uniform vec3 uLightdir;

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
    vec2 UV = iUV; UV.y = 1. - UV.y;
    vec4 base = texture(uTex, texi_to_botleft(texi) + (UV * FACE_SIZE_UV));
    if (base.a < 1) discard;
    vec4 kd = base * max(0.,dot(-uLightdir, iNorm));
    vec4 ka = base * 0.22;
    outColor = ka + kd;
}
