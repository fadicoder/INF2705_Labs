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

struct Reflexions {
    vec3 diffuse;
    vec3 specular;
};


float calculateSpot(vec3 l, vec3 n, vec3 spotDirectin) {
    float spotFacteur = 0.0;
    if (dot(spotDirectin, n) >= 0 ) {
        float dotPorduct = dot(l, spotDirectin);
        if (dotPorduct > cos(radians(spotOpeningAngle))) {
            if (useDirect3D) {
                float cosInner = cos(radians(spotOpeningAngle));
                float cosOuter = pow(cosInner, 1.01 + (spotExponent / 2));
                spotFacteur = smoothstep(cosOuter, cosInner, dotPorduct);
            } else {
                spotFacteur = pow(dotPorduct, spotExponent);
            }
        }
    }
    return spotFacteur;
}

Reflexions calculateReflexion(vec3 n, vec3 o, int spotIndex) {

    Reflexions result = Reflexions(
        vec3(0),
        vec3(0)
    );

    vec3 l = normalize(attribIn.lightDir[spotIndex]);
    float dist = length(attribIn.lightDir[spotIndex]);
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
        float spotFactor = calculateSpot(l, n, attribIn.spotDir[spotIndex]);
        result.diffuse *= spotFactor;
        result.specular *= spotFactor;
    }
    return result;
}

void main() {
    vec3 n = normalize(gl_FrontFacing ? attribIn.normal : -attribIn.normal);
    vec3 o = normalize(attribIn.obsPos);

    vec3 ambiantI = vec3(0.0, 0.0, 0.0);
    vec3 diffuseI = vec3(0.0, 0.0, 0.0);
    vec3 specularI = vec3(0.0, 0.0, 0.0);

    // Emission:
    vec3 emissionI = mat.emission;

    // Ambiant:
    ambiantI += mat.ambient * lightModelAmbient;
    for (int i = 0; i < 3; i++) {
        ambiantI += mat.ambient * lights[i].ambient;
    }

    // Diffuse & Speculaire:
    for (int i = 0; i < 3; i++) {
        Reflexions reflexions = calculateReflexion(n, o, i);
        diffuseI += reflexions.diffuse;
        specularI += reflexions.specular;
    }

    vec4 diffuseTexture = texture(diffuseSampler, attribIn.texCoords);
    if (diffuseTexture.a < 0.3) discard;
    vec3 diffuseColor = diffuseTexture.xyz * (diffuseI + ambiantI);
    vec3 specularColor = texture(specularSampler, attribIn.texCoords).xyz * specularI;
    FragColor = vec4(emissionI + specularColor + diffuseColor, 1.0);
    FragColor = clamp(FragColor, 0.0, 1.0);
}
