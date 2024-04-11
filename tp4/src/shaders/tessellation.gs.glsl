#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;


in ATTRIB_TES_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
} attribIn[];

out ATTRIB_GS_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
    vec3 barycentricCoords;
} attribOut;

void main()
{
    for (int i = 0; i < gl_in.length(); i++) {
        attribOut.texCoords = attribIn[i].texCoords;
        attribOut.height = attribIn[i].height;
        attribOut.patchDistance = attribIn[i].patchDistance;

        attribOut.barycentricCoords = vec3(0);
        attribOut.barycentricCoords[i] = 1.0;

        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
}
