#version 440

struct Material
{
	vec4 emissive;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

layout (location = 1) in vec4 colour;

layout (location = 0) out vec4 finalColour;

uniform Material material;
uniform vec4 lightColour;

void main()
{    
	vec4 lightingColour = material.emissive + lightColour;
	
	finalColour =  colour + lightingColour;
}

