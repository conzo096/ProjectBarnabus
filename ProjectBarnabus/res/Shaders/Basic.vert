#version 440

const int MAX_BONES = 100;

// Model view projection matrix
uniform mat4 MVP;

uniform mat4 gBones[MAX_BONES];

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 baseColour;
layout (location = 3) in vec2 tex_coord_in;

layout (location = 5) in ivec4 boneIDs;
layout (location = 6) in vec4  weights;

layout (location = 0) out vec2 tex_coord_out;
layout (location = 1) out vec4 baseColourOut;

void main()
{

	vec4 vertexIn = vec4(position, 1.0);

	vec4 vertexOut =
	(gBones[boneIDs[0]] * vertexIn) * weights[0] +
	(gBones[boneIDs[1]] * vertexIn) * weights[1] +
	(gBones[boneIDs[2]] * vertexIn) * weights[2] +
	(gBones[boneIDs[3]] * vertexIn) * weights[3];

	gl_Position = MVP * vec4(vertexIn.xyz,1);
	// Output texture coordinate to fragement shader
	tex_coord_out = tex_coord_in;
	
	baseColourOut = baseColour;
	//baseColourOut = normalize(vertexOut * -1);
	baseColourOut.a = 1.0;
}