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
    vec2 positions[4] = vec2[4](vec2(-1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, -1.0), vec2(1.0, 1.0));
    vec2 texPositions[4] = vec2[4](vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 0.0), vec2(1.0, 1.0));

    for (int i = 0; i < 4; i++) {
        vec2 pos2D = attribIn[i].size * positions[i] * 0.5;
        gl_Position = (gl_in[0].gl_Position + vec4(pos2D.x, pos2D.y, 0.0, 0.0)) * projection;

        attribOut.color = attribIn[i].color;
        attribOut.texCoords = texPositions[i];
        EmitVertex();
    }
}
