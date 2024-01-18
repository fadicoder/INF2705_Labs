#version 330 core

layout(location=0) in vec4 a_position;
out vec4 color;

void main(){
    color = vec4(255.0f, 255.0f, 0.0f, 1); // Jaune
}
