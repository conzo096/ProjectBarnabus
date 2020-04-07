#version 440
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 colour;
layout (location = 2) in vec3 normal;

layout (location =0) out vec3 positionOut;
layout (location = 1) out vec4 colourOut;
layout (location = 2) out vec3 normalOut;
uniform mat4 MVP;
uniform mat4 M;
void main()
{
	vec4 worldPosition = MVP * vec4(position, 1.0);
    gl_Position = worldPosition;

	positionOut = vec3(M * vec4(position, 1.0));
	colourOut = colour;
	normalOut = normal;
}

