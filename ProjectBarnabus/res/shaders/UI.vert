#version 440

// Model view projection matrix
uniform mat4 MVP;

layout (location = 0) in vec3 position;
layout (location = 3) in vec2 tex_coord_in;

layout (location = 0) out vec2 tex_coord_out;

void main()
{
	// Calculate screen position of vertex
	gl_Position = vec4(position, 1.0);
	// Output texture coordinate to fragement shader
	tex_coord_out = tex_coord_in;
}