#version 440

// Incoming texture coordinate
layout(location = 0) in vec2 tex_coord;
// Outgoing fragment colour
layout(location = 0) out vec4 colour;

void main()
{
	colour = vec4(0,1,1,1);	
}