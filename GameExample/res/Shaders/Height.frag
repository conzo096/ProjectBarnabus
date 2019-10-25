#version 440

layout (location = 0) in vec4 colour;
layout (location = 0) out vec4 finalColour;

void main()
{    
	finalColour = colour;
}

