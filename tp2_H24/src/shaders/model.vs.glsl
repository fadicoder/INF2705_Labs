#version 330 core


layout(location=0) in vec4 a_position;
layout(location=1) in vec2 a_texCoord;

out vec2 texCoord;

uniform mat4 mvp;

void main(void){
    texCoord = a_texCoord;
    gl_Position = mvp * a_position; // projection * view * model * vec4(a_position, 1.0);
}
