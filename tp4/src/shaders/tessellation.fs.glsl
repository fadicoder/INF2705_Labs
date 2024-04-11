#version 330 core

in ATTRIB_GS_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
    vec3 barycentricCoords;
} attribIn;

uniform sampler2D groundSampler;
uniform sampler2D sandSampler;
uniform sampler2D snowSampler;
uniform bool viewWireframe;

out vec4 FragColor;

float edgeFactor(vec3 barycentricCoords, float width)
{
    vec3 d = fwidth(barycentricCoords);
    vec3 f = step(d * width, barycentricCoords);
    return min(min(f.x, f.y), f.z);
}

float edgeFactor(vec4 barycentricCoords, float width)
{
    vec4 d = fwidth(barycentricCoords);
    vec4 f = step(d * width, barycentricCoords);
    return min(min(min(f.x, f.y), f.z), f.w);
}

const vec3 WIREFRAME_COLOR = vec3(0.5f);
const vec3 PATCH_EDGE_COLOR = vec3(1.0f, 0.0f, 0.0f);

const float WIREFRAME_WIDTH = 0.5f;
const float PATCH_EDGE_WIDTH = 0.5f;

void main()
{
    vec4 color = vec4(1.0);

    if (attribIn.height < 0.3){
        color = texture(sandSampler, attribIn.texCoords).rgba;
    } else if (attribIn.height < 0.35){
        vec4 sand = texture(sandSampler, attribIn.texCoords).rgba;
        vec4 ground = texture(groundSampler, attribIn.texCoords).rgba;
        color = mix(sand, ground, smoothstep(0.3, 0.35, attribIn.height)); // melange de sand et de ground
    } else if (attribIn.height < 0.6 ){
        color = texture(groundSampler, attribIn.texCoords).rgba;
    } else if (attribIn.height < 0.65){
        vec4 ground = texture(groundSampler, attribIn.texCoords).rgba;
        vec4 snow = texture(snowSampler, attribIn.texCoords).rgba;
        color = mix(ground, snow, smoothstep(0.6, 0.65, attribIn.height)); // melange de sand et de ground
    } else {
        color = texture(snowSampler, attribIn.texCoords).rgba;
    }
    FragColor = color;

    if (viewWireframe) {
        float factor = edgeFactor(attribIn.barycentricCoords, WIREFRAME_WIDTH);
        if (factor == 0) {
            FragColor = vec4(mix(WIREFRAME_COLOR, color.xyz, 0.5), 1.0f);
        }
        factor = edgeFactor(attribIn.patchDistance, PATCH_EDGE_WIDTH);
        if (factor == 0) {
            FragColor = vec4(PATCH_EDGE_COLOR, 1.0f);
        }
    }
}
