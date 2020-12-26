#version 440

layout (binding = 1) uniform sampler2D font;

layout(location = 0) in vec2 UV;

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(vec3(texture( font, UV ) * vec4(0,0,0,1)),texture( font, UV ).a);
}