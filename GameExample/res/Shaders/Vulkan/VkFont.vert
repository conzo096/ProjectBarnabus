#version 440
layout(location = 0) in vec2 position;
layout(location = 3) in vec2 texCoordIn;

layout(location = 0) out vec2 texCoordOut;

void main()
{
	// map to [-1..1][-1..1]
	vec2 vertexPosition = position - vec2(1920,1080);
	vertexPosition /= vec2(1920,1080);
	gl_Position =  vec4(vertexPosition,0,1);
	
	texCoordOut = texCoordIn;
}