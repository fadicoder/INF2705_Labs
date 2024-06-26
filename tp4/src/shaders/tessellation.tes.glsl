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

vec2 interpole( vec2 v0, vec2 v1, vec2 v2, vec2 v3 ) { // L'interpolation est seulement en 2D
    vec2 v01 = mix(v0, v1, gl_TessCoord.x);
    vec2 v32 = mix(v3, v2, gl_TessCoord.x);
    return mix(v01, v32, gl_TessCoord.y);
}


const float PLANE_SIZE = 256.0f;

void main() {
    vec2 p0 = gl_in[0].gl_Position.xz;
    vec2 p1 = gl_in[1].gl_Position.xz;
    vec2 p2 = gl_in[2].gl_Position.xz;
    vec2 p3 = gl_in[3].gl_Position.xz;

    vec2 interpoled_pos = clamp(interpole(p0, p1, p2, p3), -PLANE_SIZE/2.0, PLANE_SIZE/2.0); // On s'assure que la coordonnée de monde est dans le plain
    vec2 pos_2d = (interpoled_pos / PLANE_SIZE) + vec2(0.5); // Transformer la coordonnée de monde pour être entre 0 et 1
    float z = texture(heighmapSampler, pos_2d / 4.0).x; // Si on utilise gl_TessCoord au lieu de pos_2d/4.0 ca donne un meilleur résultat

    float MAX_HEIGHT = 32.0;
    vec4 pos = vec4(interpoled_pos.x, (z * MAX_HEIGHT * 2) - MAX_HEIGHT, interpoled_pos.y, 1.0f);
    gl_Position = mvp * pos;

    attribOut.height = z;
    attribOut.texCoords = (gl_TessCoord * 2).xy;
    attribOut.patchDistance = vec4(
        gl_TessCoord.x,
        gl_TessCoord.y,
        1.0 - gl_TessCoord.x,
        1.0 - gl_TessCoord.y);
}
