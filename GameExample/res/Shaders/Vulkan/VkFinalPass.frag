#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (binding = 1) uniform sampler2D samplerAlbedo;

layout (location = 0) in vec2 tex_coord_in;

layout(location = 0) out vec4 outColor;

void main()
{
	vec4 albedo = texture(samplerAlbedo, tex_coord_in);
	albedo.a = 1;
    outColor = albedo;
}