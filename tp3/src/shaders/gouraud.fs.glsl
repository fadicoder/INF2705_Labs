#version 330 core

in ATTRIB_VS_OUT
{
    vec2 texCoords;
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} attribIn;

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

out vec4 FragColor;

void main()
{
    vec4 diffuseTex = texture(diffuseSampler, attribIn.texCoords);
    if (diffuseTex.a < 0.3) discard;
    vec3 diffuseColor = diffuseTex.xyz * (attribIn.diffuse + attribIn.ambient);
    vec3 specularColor = texture(specularSampler, attribIn.texCoords).xyz * attribIn.specular;
    FragColor = vec4(attribIn.emission + specularColor + diffuseColor, 1.0);
}
