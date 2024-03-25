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
    vec3 side1 = attribIn[1].position - attribIn[0].position;
    vec3 side2 = attribIn[2].position - attribIn[0].position;
    vec3 normal = normalize(cross(side1 , side2));

    vec3 center = (attribIn[0].position + attribIn[1].position + attribIn[2].position) / 3.0;
    vec3 pos = vec3(modelView * vec4(center, 1));

    // emission:
    attribOut.emission = mat.emission;

    // ambiant:
    attribOut.ambient = mat.ambient * lightModelAmbient;
    for (int j = 0; j < 3; j++) {
        attribOut.ambient += (mat.ambient * lights[j].ambient);
    }

    // Diffuse & Speculaire:
    attribOut.specular = vec3(0.0, 0.0, 0.0);
    attribOut.diffuse = vec3(0.0, 0.0, 0.0);
    vec3 o = normalize(-pos);
    for (int j = 0; j < 3; j++) {
        vec3 lightDirection = (modelView * vec4(lights[j].position, 1.0f)).xyz - pos;
        vec3 spotDirection = mat3(view) * -lights[j].spotDirection;
        Reflexions reflexions = calculateReflexion(normal, o, lightDirection, spotDirection, j);
        attribOut.diffuse += reflexions.diffuse;
        attribOut.specular += reflexions.specular;
    }

    // Emitting points
    for (int i = 0; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        attribOut.texCoords = attribIn[i].texCoords;
        EmitVertex();
    }
}
