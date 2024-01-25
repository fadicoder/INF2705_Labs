#version 330 core

layout(location=0) in vec4 a_position;
out vec4 color;

void main(void){
    gl_Position = a_position;
}
