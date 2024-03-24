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


float calculateSpot(vec3 l, vec3 n, vec3 spotDirectin) {
    float spotFacteur = 0.0;
    if (dot(spotDirectin, n) >= 0 ) {
        float spotDot = dot(l, spotDirectin);
        if (spotDot > cos(radians(spotOpeningAngle))) {
            spotFacteur = pow(spotDot, spotExponent);
        }
    }
    return spotFacteur;
}

void calculateReflexion(vec3 n, vec3 o, vec3[3] lightDir, vec3[3] spotDir, int spotIndex) {
    vec3 l = normalize(lightDir[spotIndex]);
    float dist = length(lightDir[spotIndex]);

    float distFactor = min(1.0 / (dist*dist), 1.0);
    float dotProd = dot(n, l);

    // seulment calucler diffuse et speculaire quand la face est éclairée.
    if (dotProd > 0.0) {
        // Diffuse:
        attribOut.diffuse += (mat.diffuse * lights[spotIndex].diffuse * dotProd) * distFactor;

        // Spectulaire:
        // Calculer l'intensité de la réflection spéculaire selon la formule de Blinn ou Phong. Elle dépend de la position de la lumière, de la normale et de la position de l'observateur.
        float specIntensity = useBlinn ? dot(normalize(l + o), n) : dot(reflect(-l, n), o);

        // Si le résultat est positif (il y a de la réflexion spéculaire).
        if (specIntensity > 0) {
            float shine = pow(specIntensity, mat.shininess);
            attribOut.specular += (shine * mat.specular * lights[spotIndex].specular) * distFactor;
        }
    }
    if (useSpotlight) {
        attribOut.diffuse *= calculateSpot(l, n, spotDir[spotIndex]);
        attribOut.specular *= calculateSpot(l, n, spotDir[spotIndex]);
    }
}



void main() {
    // La position du sommet dans le référentiel de la caméra (donc coords de visualisation).
    vec3 transformedNormal = normalMatrix * normal;
    vec3 lightDir[3];
    vec3 spotDir[3];
    vec3 pos = (modelView * vec4(position, 1.0)).xyz;
    vec3 obsPos = normalize(-pos);

    lightDir[0] = (view * vec4(lights[0].position, 1.0f)).xyz - pos ;
    lightDir[1] = (view * vec4(lights[1].position, 1.0f)).xyz - pos ;
    lightDir[2] = (view * vec4(lights[2].position, 1.0f)).xyz - pos ;

    spotDir[0] = mat3(view) * -lights[0].spotDirection;
    spotDir[1] = mat3(view) * -lights[1].spotDirection;
    spotDir[2] = mat3(view) * -lights[2].spotDirection;

    gl_Position = mvp * vec4(position, 1.0);
    attribOut.texCoords = texCoords;

    // Emission
    attribOut.emission = mat.emission;

    // Ambiant
    attribOut.ambient = mat.ambient * lightModelAmbient;
    for (int j = 0; j < 3; j++) {
        attribOut.ambient += mat.ambient * lights[j].ambient;
    }

    // Diffuse & specular
    attribOut.diffuse = vec3(0.0, 0.0, 0.0);
    attribOut.specular = vec3(0.0, 0.0, 0.0);
    for (int j = 0; j < 3; j++) {
        calculateReflexion(transformedNormal, obsPos, lightDir, spotDir, j);
    }
}
