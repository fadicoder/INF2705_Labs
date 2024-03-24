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
    vec3 color = attribIn.emission + attribIn.ambient + attribIn.diffuse + attribIn.specular;
    FragColor = vec4(color, 1.0);
}
