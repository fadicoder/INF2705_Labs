#version 330 core

uniform sampler2D texSampler;

in vec2 texCoord;
out vec3 fragColor;

void main(void){
    fragColor = texture(texSampler, texCoord.st).rgb;
}