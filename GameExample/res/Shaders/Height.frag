#version 440

uniform sampler2D heightTexture;

layout(location = 0) in vec2 texCoord;

layout (location = 0) out vec4 finalColour;

void main()
{    
	finalColour = texture(heightTexture, texCoord);
}

