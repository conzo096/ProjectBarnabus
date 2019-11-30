#version 440

uniform sampler2D gameTexture;
uniform sampler2D uiTexture;

// Incoming texture coordinate
layout(location = 0) in vec2 tex_coord;
// Outgoing fragment colour
layout(location = 0) out vec4 colour;

void main()
{
	vec4 gameTextureColour = texture(gameTexture, tex_coord);
	vec4 uiTextureColour = texture(uiTexture, tex_coord);
	colour = mix(gameTextureColour, uiTextureColour, uiTextureColour.a);

	//colour = uiTextureColour;
}