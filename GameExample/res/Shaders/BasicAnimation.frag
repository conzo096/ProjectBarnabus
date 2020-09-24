#version 440

uniform sampler2D tex;

// Incoming texture coordinate
layout(location = 0) in vec2 texCoord;
layout (location = 1) in vec4 baseColour;
// Outgoing fragment colour
layout(location = 0) out vec4 colour;

void main()
{
	colour = texture(tex, texCoord);
	colour.a = 1;
}