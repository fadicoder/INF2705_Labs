#version 330 core

uniform samplerCube texSampler;

in vec3 texCoord;
out vec3 fragColor;


void main(void){
    fragColor = texture(texSampler, texCoord).rgb;
}