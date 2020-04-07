#version 440

struct DirectionalLight
{
	vec4 colour;
	vec3 direction;
};

struct Material
{
	vec4 emissive;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

layout (location = 1) in vec4 colour;
layout (location = 2) in vec3 normal;

layout (location = 0) out vec4 finalColour;

uniform Material material;
uniform DirectionalLight worldLight;

void main()
{    
	float intensity = worldLight.direction * normal;

	vec4 lightingColour = material.emissive + (material.diffuse * worldLight.colour) * intensity;
	
	finalColour =  colour + lightingColour;
}

