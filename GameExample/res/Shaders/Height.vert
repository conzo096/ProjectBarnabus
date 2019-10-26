#version 440
layout (location = 0) in vec3 position;

layout (location = 0) out vec4 colour;

uniform mat4 MVP;

void main()
{
	vec4 worldPosition = MVP * vec4(position, 1.0);
    gl_Position = worldPosition;

	vec4 height = mix(vec4(0,0,0,1),vec4(1,1,1,1), mod(((position.y/255)*2),12));
	colour = height;
}

