#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (binding = 1) uniform sampler2D gameTexture;
layout (binding = 2) uniform sampler2D uiTexture;

layout (location = 0) in vec2 tex_coord_in;

layout(location = 0) out vec4 outColor;

void main()
{
	vec4 gameTextureColour = texture(gameTexture, tex_coord_in);
	vec4 uiTextureColour = texture(uiTexture, tex_coord_in);

	outColor = mix(gameTextureColour, uiTextureColour, uiTextureColour.a);
}