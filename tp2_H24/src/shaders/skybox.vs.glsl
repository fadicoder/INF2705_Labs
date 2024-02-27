#version 330 core


layout(location=0) in vec3 a_position;

out vec3 texCoord;

uniform mat4 mvp;

void main(void){
    texCoord = a_position;
    vec4 worldPos = mvp * vec4(a_position, 1.0f);
    gl_Position = vec4(worldPos.x, worldPos.y, worldPos.w, worldPos.w);
}
