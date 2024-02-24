#version 330 core

uniform sampler2D texSampler;

in vec2 texCoord;
out vec3 fragColor;


void main(void){
    vec4 texture = texture(texSampler, texCoord.st);
    if (texture.a < 0.3) discard;
    fragColor = texture.rgb;
}