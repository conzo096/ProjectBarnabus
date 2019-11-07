#version 440
layout (location = 0) in vec3 position;
layout (location = 3) in vec2 texCoordIn;

layout (location = 0) out vec2 texCoordOut;
uniform mat4 MVP;

void main()
{
	vec4 worldPosition = MVP * vec4(position, 1.0);
    gl_Position = worldPosition;

	texCoordOut = texCoordIn;
}

