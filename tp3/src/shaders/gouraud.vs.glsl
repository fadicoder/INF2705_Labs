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

struct Reflexions {
    vec3 diffuse;
    vec3 specular;
};


float calculateSpot(vec3 l, vec3 n, vec3 spotDirection) {
    float spotFacteur = 0.0;
    if (dot(spotDirection, n) >= 0 ) {
        float dotProduct = dot(l, spotDirection);
        if (dotProduct > cos(radians(spotOpeningAngle))) {
            if (useDirect3D) {
                float cosInner = cos(radians(spotOpeningAngle));
                float cosOuter = pow(cosInner, 1.01 + (spotExponent / 2.0));
                spotFacteur = smoothstep(cosOuter, cosInner, dotProduct);
            } else {
                spotFacteur = pow(dotProduct, spotExponent);
            }
        }
    }
    return spotFacteur;
}

Reflexions calculateReflexion(vec3 n, vec3 o, vec3 lightDir, vec3 spotDir, int spotIndex) {

    Reflexions result = Reflexions(
        vec3(0),
        vec3(0)
    );

    vec3 l = normalize(lightDir);
    float dist = length(lightDir);
    float distFactor = min(1.0 / (dist*dist), 1.0);
    float dotProd = dot(n, l);

    // seulment calucler diffuse et speculaire quand la face est éclairée.
    if (dotProd > 0.0) {
        // Diffuse:
        result.diffuse = (mat.diffuse * lights[spotIndex].diffuse * dotProd) * distFactor;

        // Spectulaire:
        // Calculer l'intensité de la réflection spéculaire selon la formule de Blinn ou Phong. Elle dépend de la position de la lumière, de la normale et de la position de l'observateur.
        float specIntensity = useBlinn ? dot(normalize(l + o), n) : dot(reflect(-l, n), o);

        // Si le résultat est positif (il y a de la réflexion spéculaire).
        if (specIntensity > 0) {
            float shine = pow(specIntensity, mat.shininess);
            result.specular = (shine * mat.specular * lights[spotIndex].specular) * distFactor;
        }
    }

    // spot:
    if (useSpotlight) {
        float spotFactor = calculateSpot(l, n, spotDir);
        result.diffuse *= spotFactor;
        result.specular *= spotFactor;
    }
    return result;
}



void main() {
    gl_Position = mvp * vec4(position, 1.0);
    attribOut.texCoords = texCoords;

    // La position du sommet dans le référentiel de la caméra (donc coords de visualisation).
    vec3 transformedNormal = normalMatrix * normal;
    vec3 pos = (modelView * vec4(position, 1.0)).xyz;
    vec3 obsPos = -pos;

    // Emission
    attribOut.emission = mat.emission;

    // Ambiant
    attribOut.ambient = mat.ambient * lightModelAmbient;
    for (int i = 0; i < 3; i++) {
        attribOut.ambient += (mat.ambient * lights[i].ambient);
    }

    // Diffuse & specular
    attribOut.diffuse = vec3(0.0, 0.0, 0.0);
    attribOut.specular = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < 3; i++) {
        vec3 lightDir = (view * vec4(lights[i].position, 1.0f)).xyz - pos;
        vec3 spotDir = mat3(view) * -lights[i].spotDirection;
        Reflexions reflexions = calculateReflexion(transformedNormal, normalize(obsPos), lightDir, spotDir, i);
        attribOut.diffuse += reflexions.diffuse;
        attribOut.specular += reflexions.specular;
    }
}
