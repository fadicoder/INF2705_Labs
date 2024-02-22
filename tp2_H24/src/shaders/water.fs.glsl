#version 330 core

uniform sampler2D texSampler;

in vec2 texCoord;
out vec3 fragColor;
uniform uint appTime;

void main(void){
    float time = appTime / 1000.0f;
    float x = 0.025 * cos(time + ((texCoord.x + texCoord.y) * 2));
    float y = 0.01 * sin((time * 2) + ((texCoord.x + texCoord.y) * 2)) + (time * 0.2);
    fragColor = texture(texSampler, texCoord.st + vec2(x, y)).rgb;
}