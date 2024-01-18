#version 330 core

layout(location=0) in vec4 a_position;
out vec4 gl_Position; // déclaration implicite

void main(){
    gl_Position = a_position;
}
