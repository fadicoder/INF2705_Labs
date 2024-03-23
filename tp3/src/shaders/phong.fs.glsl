#version 330 core

in ATTRIB_VS_OUT
{
    vec2 texCoords;
    vec3 normal;
    vec3 lightDir[3];
    vec3 spotDir[3];
    vec3 obsPos;
} attribIn;

struct Reflections
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

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

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

out vec4 FragColor;

float calculateSpot(vec3 l, vec3 n, int spotIndex) {
    float spotFacteur = 0.0;
    vec3 d = attribIn.spotDir[spotIndex];
    if (dot(d, n) >= 0 ) {
        float spotDot = dot(l, d);
        if (spotDot > cos(radians(spotOpeningAngle))) {
            spotFacteur = pow(spotDot, spotExponent);
        }
    }
    return spotFacteur;
}

void main() {
    // Emission;
    vec3 specAndDiffuse = vec3(0.0, 0.0, 0.0);
    vec3 color = mat.emission;

    // Ambiant:
    color += mat.ambient * lightModelAmbient;
    color += mat.ambient * lights[0].ambient;

    // calculs nécessaires pour diffuse et spéculaire:
    vec3 l = normalize(attribIn.lightDir[0]);
    vec3 n = normalize(gl_FrontFacing ? attribIn.normal : -attribIn.normal);
    vec3 o = normalize(attribIn.obsPos);
    float dist = length(attribIn.lightDir[0]);
    float distFactor = min(1.0 / (dist*dist), 1.0);
    float dotProd = dot(n, l);

    // seulment calucler diffuse et speculaire quand la face est éclairée.
    if (dotProd > 0.0) {
        // Diffuse:
        specAndDiffuse += (mat.diffuse * lights[0].diffuse * dotProd) * distFactor;

        // Spectulaire:
        // Calculer l'intensité de la réflection spéculaire selon la formule de Blinn ou Phong. Elle dépend de la position de la lumière, de la normale et de la position de l'observateur.
        float specIntensity = useBlinn ? dot(normalize(l + o), n) : dot(reflect(-l, n), o);

        // Si le résultat est positif (il y a de la réflexion spéculaire).
        if (specIntensity > 0) {
            float shine = pow(specIntensity, mat.shininess);
            specAndDiffuse += (shine * mat.specular * lights[0].specular) * distFactor;
        }
    }

    if (useSpotlight) {
        specAndDiffuse *= calculateSpot(l, n, 0);
    }

    FragColor = vec4(color + specAndDiffuse, 1.0f);
    FragColor = clamp(FragColor, 0.0, 1.0);
}
