#version 440

layout(location = 0) in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D font;

void main()
{
	color = vec4(vec3(texture( font, UV ) * vec4(0,0,0,1)),texture( font, UV ).a);
}