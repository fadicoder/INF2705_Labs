#version 400 core

layout(quads) in;

/*
in Attribs {
    vec4 couleur;
} AttribsIn[];*/


out ATTRIB_TES_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
} attribOut;

uniform mat4 mvp;

uniform sampler2D heighmapSampler;

vec2 interpole( vec4 v0, vec4 v1, vec4 v2, vec4 v3 ) {
    vec4 v01 = mix(v0, v1, gl_TessCoord.x);
    vec4 v32 = mix(v3, v2, gl_TessCoord.x);
    return mix(v01, v32, gl_TessCoord.y).xy;
}


const float PLANE_SIZE = 256.0f;

void main() {
    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec4 p2 = gl_in[2].gl_Position;
    vec4 p3 = gl_in[3].gl_Position;

    vec2 pos_2d = interpole(p0, p1, p2, p3);
    float height = texture(heighmapSampler, pos_2d / 4.0).x;
    vec4 pos = vec4(pos_2d, (height * 64.0) - 32.0, 1.0f);

    gl_Position = mvp * pos;
    attribOut.height = height;
    attribOut.texCoords = (gl_TessCoord * 2).xy;
    attribOut.patchDistance = vec4(gl_TessCoord.x, gl_TessCoord.y, 1.0 - gl_TessCoord.x, 1.0 - gl_TessCoord.y);
}