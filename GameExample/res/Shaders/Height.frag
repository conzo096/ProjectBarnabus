#version 440

uniform sampler2D heightTexture;

layout(location = 0) in vec2 texCoord;
layout (location = 1) in vec4 colour;

layout (location = 0) out vec4 finalColour;

void main()
{    
	finalColour = colour;//texture(heightTexture, texCoord);
}

