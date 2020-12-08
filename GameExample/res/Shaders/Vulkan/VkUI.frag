#version 440

layout (binding = 1) uniform sampler2D samplerAlbedo;

// Incoming texture coordinate
layout(location = 0) in vec2 tex_coord;
// Outgoing fragment colour
layout(location = 0) out vec4 colour;

void main()
{
	vec4 albedo = texture(samplerAlbedo, tex_coord);
	albedo.a = 1;
    colour = albedo;
}