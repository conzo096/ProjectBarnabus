#version 440

const int MAX_BONES = 100;

// Model view projection matrix
uniform mat4 MVP;

uniform mat4 bones[MAX_BONES];

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

	mat4 boneTransform = bones[boneIDs[0]] * weights[0];
		boneTransform += bones[boneIDs[1]] * weights[1];
		boneTransform += bones[boneIDs[2]] * weights[2];
		boneTransform += bones[boneIDs[3]] * weights[3];
			
	vec4 vertexOut = boneTransform * vertexIn; 

	gl_Position = MVP * vec4(vertexOut.xyz,1);
	// Output texture coordinate to fragement shader
	tex_coord_out = tex_coord_in;
	
	float dist = distance(vertexOut, vertexIn);
	baseColourOut = normalize(vec4(dist,dist,dist,1));
	baseColourOut.a = 1.0;
}