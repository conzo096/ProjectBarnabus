#version 450
#extension GL_ARB_separate_shader_objects : enable

const int MAX_BONES = 100;

layout(binding = 0) uniform UniformBufferObject 
{
    mat4 MVP;
	mat4 bones[MAX_BONES];
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;

layout (location = 4) in ivec4 boneIDs;
layout (location = 5) in vec4  weights;

layout(location = 0) out vec4 fragColor;

void main() 
{
	vec4 vertexIn = vec4(inPosition, 1.0);

	mat4 boneTransform = ubo.bones[boneIDs[0]] * weights[0];
		boneTransform += ubo.bones[boneIDs[1]] * weights[1];
		boneTransform += ubo.bones[boneIDs[2]] * weights[2];
		boneTransform += ubo.bones[boneIDs[3]] * weights[3];
			
	vec4 vertexOut = boneTransform * vertexIn; 

	gl_Position = ubo.MVP * vec4(inPosition.xyz,1);

	fragColor.xyz = vertexOut.xyz;
	fragColor.a = 1.0f;
}

