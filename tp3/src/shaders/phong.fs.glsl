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

vec3 calculateReflexion(vec3 n, vec3 o, int spotIndex) {
    vec3 l = normalize(attribIn.lightDir[spotIndex]);
    float dist = length(attribIn.lightDir[spotIndex]);
    float distFactor = min(1.0 / (dist*dist), 1.0);
    float dotProd = dot(n, l);
    vec3 reflexionColor = vec3(0.0, 0.0, 0.0);
    // seulment calucler diffuse et speculaire quand la face est éclairée.
    if (dotProd > 0.0) {
        // Diffuse:
        reflexionColor += (mat.diffuse * lights[spotIndex].diffuse * dotProd) * distFactor;

        // Spectulaire:
        // Calculer l'intensité de la réflection spéculaire selon la formule de Blinn ou Phong. Elle dépend de la position de la lumière, de la normale et de la position de l'observateur.
        float specIntensity = useBlinn ? dot(normalize(l + o), n) : dot(reflect(-l, n), o);

        // Si le résultat est positif (il y a de la réflexion spéculaire).
        if (specIntensity > 0) {
            float shine = pow(specIntensity, mat.shininess);
            reflexionColor += (shine * mat.specular * lights[spotIndex].specular) * distFactor;
        }
    }
    if (useSpotlight) {
        reflexionColor *= calculateSpot(l, n, spotIndex);
    }
    return reflexionColor;
}

void main() {
    vec3 n = normalize(gl_FrontFacing ? attribIn.normal : -attribIn.normal);
    vec3 o = normalize(attribIn.obsPos);

    vec3 emissionColor = mat.emission;
    vec3 reflexionColor = vec3(0.0, 0.0, 0.0);
    vec3 ambiantColor = vec3(0.0, 0.0, 0.0);

    // Ambiant:
    ambiantColor += mat.ambient * lightModelAmbient;
    ambiantColor += mat.ambient * lights[0].ambient;
    ambiantColor += mat.ambient * lights[1].ambient;
    ambiantColor += mat.ambient * lights[2].ambient;

    // diffuse et speculaire
    reflexionColor += calculateReflexion(n, o, 0);
    reflexionColor += calculateReflexion(n, o, 1);
    reflexionColor += calculateReflexion(n, o, 2);

    FragColor = vec4(emissionColor + ambiantColor + reflexionColor, 1.0f);
    FragColor = clamp(FragColor, 0.0, 1.0);
}
