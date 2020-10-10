#include "VkBasicShader.h"
#include "GameEngine/BarnabusGameEngine.h"
#include "GameEngine/VulkanUtils.h"
void VkBasicShader::Use()
{
	VulkanUtils::UpdateUniformBuffer<UniformBufferObject>(device, bufferSize, uniformBuffersMemory[uniformBufferIndex], uniformBuffers[uniformBufferIndex], uniforms);
	uniforms.clear();
}

void VkBasicShader::UpdateUniforms(MeshData& meshData)
{
	UniformBufferObject ubo{ BarnabusGameEngine::Get().GetRenderer()->GetCameraVP() * glm::mat4(meshData.GetTransform()) };

	uniforms.push_back(ubo);
}
