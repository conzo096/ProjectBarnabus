#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D font;
uniform vec4 texColour;

void main()
{
	color = vec4(vec3(texture( font, UV ) * texColour),texture( font, UV ).a);
	//color = texColour;
}