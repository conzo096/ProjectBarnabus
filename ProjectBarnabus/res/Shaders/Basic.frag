#version 440

const vec2 lightBias = vec2(0.7, 0.6);//just indicates the balance between diffuse and ambient lighting

//layout (location = 0) in vec2 pass_textureCoords;
//layout (location = 1) in vec3 pass_normal;

layout (location = 0) out vec4 out_colour;

uniform sampler2D diffuseMap;
uniform vec3 lightDirection;

void main()
{
	
	//vec4 diffuseColour = texture(diffuseMap, pass_textureCoords);		
	//vec3 unitNormal = normalize(pass_normal);
	//float diffuseLight = max(dot(-lightDirection, unitNormal), 0.0) * lightBias.x + lightBias.y;
	out_colour = vec4(1,0,0,1);
}