#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;


in ATTRIB_VS_OUT
{
    vec4 color;
    vec2 size;
} attribIn[];

out ATTRIB_GS_OUT
{
    vec4 color;
    vec2 texCoords;
} attribOut;

uniform mat4 projection;

void main()
{
    for (int i = 0; i < 4; i++) {
        attribOut.color = attribIn[i].color;
        attribOut.texCoords = attribIn[i].size;
        EmitVertex();
    }
}
