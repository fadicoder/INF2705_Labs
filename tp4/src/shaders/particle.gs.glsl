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
    vec2 positions[4] = vec2[4](vec2(-0.5, -0.5), vec2(-0.5, 0.5), vec2(0.5, -0.5), vec2(0.5, 0.5));
    for (int j = 0; j < 4; j++) {
        gl_Position = projection * (gl_in[0].gl_Position + vec4(attribIn[0].size * positions[j], 0.0, 1.0));

        attribOut.color = attribIn[0].color;
        attribOut.texCoords = positions[j] + 0.5;
        EmitVertex();
    }
    EndPrimitive();
}
