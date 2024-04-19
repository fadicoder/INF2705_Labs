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
    for (int i = 0; i < gl_in.length(); i++) {
        for (int j = 0; j < 4; j++) {
            vec3 pos = vec3(gl_in[i].gl_Position.xy + (attribIn[i].size * positions[j] * 0.5), gl_in[i].gl_Position.z);
            gl_Position = projection * vec4(pos, 1.0);

            attribOut.color = attribIn[i].color;
            attribOut.texCoords = texPositions[j];
            EmitVertex();
        }
    }
}
