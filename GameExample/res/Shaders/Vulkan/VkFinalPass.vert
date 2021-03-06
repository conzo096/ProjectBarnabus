#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout (location = 3) in vec2 tex_coord_in;

layout (location = 0) out vec2 tex_coord_out;

void main()
{
	gl_Position = vec4(inPosition, 1.0);
	gl_Position.y = -gl_Position.y;
	tex_coord_out = tex_coord_in;
}