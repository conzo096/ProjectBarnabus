#version 440

struct Material
{
	vec4 emissive;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

layout (location = 1) in vec4 inColour;

layout (location = 0) out vec4 colour;

uniform Material material;

void main()
{    
	colour = inColour * material.emissive;
}

