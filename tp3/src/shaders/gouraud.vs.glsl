#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

out ATTRIB_VS_OUT
{
    vec2 texCoords;
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} attribOut;

uniform mat4 mvp;
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
    gl_Position = mvp * vec4(position, 1.0);
    attribOut.texCoords = texCoords;

    // Calculate emission: I think it is ok, it does not give me any error
    attribOut.emission = mat.emission;
    vec3 sum = lights[0].ambient;
    for (int j = 1; j < 3; j++) {
        sum += lights[j].ambient;
    }
    attribOut.ambient = sum + mat.ambient;

    sum = lights[0].diffuse;
    for (int j = 1; j < 3; j++) {
        sum += lights[j].diffuse;
    }
    attribOut.diffuse = sum + mat.diffuse;

    sum = lights[0].specular;
    for (int j = 1; j < 3; j++) {
        sum += lights[j].specular;
    }
    attribOut.specular = sum + mat.specular;
}
