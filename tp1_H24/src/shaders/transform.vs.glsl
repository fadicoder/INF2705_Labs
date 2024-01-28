#version 330 core

uniform in mat4 mvp;
layout(location=0) in vec4 a_position;
layout(location=1) in vec4 a_color;

out vec4 color;

void main(void){
    gl_Position = a_position * mvp;
    color = a_color;
}
