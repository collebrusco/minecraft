#version 410 core
out vec4 outColor;

in  vec3 iNorm;
in  vec2 iUV; 

uniform sampler2D uTex;
uniform vec3 uLightdir;


void main(){
    vec2 UV = iUV; UV.y = 1. - UV.y;
    vec4 base = texture(uTex, UV);
    vec4 kd = base * max(0.,dot(-uLightdir, iNorm));
    vec4 ka = base * 0.22;
    outColor = ka + kd;
}
