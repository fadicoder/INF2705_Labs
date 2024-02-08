#version 330 core


layout(location=0) in vec4 a_position;

uniform mat4 mvp;

void main(void){
    gl_Position = mvp * a_position; // projection * view * model * vec4(a_position, 1.0);
}
