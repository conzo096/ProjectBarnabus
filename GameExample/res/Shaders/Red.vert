#version 440
layout (location = 0) in vec3 position;

out vec4 colour;
uniform mat4 MVP;

void main()
{
	vec4 worldPosition = MVP * vec4(position, 1.0);
    gl_Position = worldPosition;
}

