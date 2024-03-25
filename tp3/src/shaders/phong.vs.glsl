#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

out ATTRIB_VS_OUT
{
    vec2 texCoords;
    vec3 normal;
    vec3 lightDir[3];
    vec3 spotDir[3];
    vec3 obsPos;
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

void main() {
    gl_Position = mvp * vec4(position, 1.0f);;
    attribOut.texCoords = texCoords;

    // Calculer la normale du sommet en appliquant l'inverse transposée de la matrice modèle-vue.
    attribOut.normal = normalMatrix * normal;

    // La position du sommet dans le référentiel de la caméra (donc coords de visualisation).
    vec3 pos = (modelView * vec4(position, 1.0f)).xyz;

    // Calculer la position de la lumière en coords de visualisation (light.position est en coordonnées de scène).
    for (int i = 0; i < 3; i++) {
        attribOut.lightDir[i] = (modelView * vec4(lights[i].position, 1.0f)).xyz - pos;
        attribOut.spotDir[i] = mat3(modelView) * -lights[0].spotDirection;
    }

    attribOut.obsPos = -pos;
}
