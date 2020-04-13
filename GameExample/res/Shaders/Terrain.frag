#version 440

struct DirectionalLight
{
	vec4 colour;
	vec3 position;
};

struct Material
{
	vec4 emissive;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 colour;
layout (location = 2) in vec3 normal;

layout (location = 0) out vec4 finalColour;

uniform Material material;
uniform DirectionalLight worldLight;

void main()
{    
	float intensity = (dot(normal,normalize(worldLight.position - position)));

	vec4 lightingColour = (worldLight.colour * intensity);
	
	finalColour =  colour + lightingColour;
}

