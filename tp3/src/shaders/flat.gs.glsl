#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in ATTRIB_OUT
{
    vec3 position;
    vec2 texCoords;
} attribIn[];

out ATTRIB_VS_OUT
{
    vec2 texCoords;
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} attribOut;

uniform mat4 view;
uniform mat4 modelView;
uniform mat3 normalMatrix;

struct Material
{
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct UniversalLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    vec3 spotDirection;
};

layout (std140) uniform LightingBlock
{
    Material mat;
    UniversalLight lights[3];
    vec3 lightModelAmbient;
    bool useBlinn;
    bool useSpotlight;
    bool useDirect3D;
    float spotExponent;
    float spotOpeningAngle;
};


void main()
{
//    vec3 side1 = gl_in[1].gl_Position - gl_in[0].gl_Position;
//    vec3 side2 = gl_in[2].gl_Position - gl_in[0].gl_Position;
//    vec3 normal = cross(side1, side2);
    for (int i = 0; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        attribOut.texCoords = attribIn[i].texCoords;

        // Calculate emission
        vec3 emissionFactor = vec3(1.0);
        if(useSpotlight) {
            vec3 cosY = cos(normalMatrix * lights[i].spotDirection);
            if (useDirect3D) {
                emissionFactor = smoothstep(pow(cos(spotOpeningAngle), (1.01 + spotExponent / 2)), cos(spotOpeningAngle), cosY);
            } else {
                emissionFactor = vec3(pow(cosY.x, spotExponent),pow(cosY.y, spotExponent), pow(cosY.z, spotExponent));
            }
        }
        attribOut.emission = mat.emission * emissionFactor;

        vec3 sum = lights[0].ambient;
        for (int j = 1; j < 3; j++) {
            sum += lights[j].ambient;
        }
        attribOut.ambient = sum * mat.ambient;

        sum = lights[0].diffuse;
        for (int j = 1; j < 3; j++) {
            sum += lights[j].diffuse;
        }
        attribOut.diffuse = sum * mat.diffuse;

        sum = lights[0].specular;
        for (int j = 1; j < 3; j++) {
            sum += lights[j].specular;
        }
        attribOut.specular = sum * mat.specular;
        EmitVertex();
    }
}
