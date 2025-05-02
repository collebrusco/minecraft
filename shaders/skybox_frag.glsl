#version 410 core

out vec4 outColor;

in vec3 iPos;

uniform samplerCube uTex;


void main(){
    outColor = texture(uTex, iPos);
}
