#include "VulkanShader.h"
#include "BarnabusGameEngine.h"
#include "VulkanRenderer.h"
#include "VulkanUtils.h"

#include <fstream>
#include <array>

namespace
{
	std::vector<char> ReadFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) 
		{
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	VkShaderModule CreateShaderModule(const std::vector<char>& code, VkDevice device)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create shader module!");
		}

		return shaderModule;
	}
} // namespace

VulkanShader::VulkanShader() : VulkanShader({MeshData::TRIANGLE, MeshData::LINE_STRIP})
{
}

VulkanShader::VulkanShader(std::set<MeshData::PrimativeType> types) : primatives(types)
{
	auto renderer = static_cast<VulkanRenderer*>(BarnabusGameEngine::Get().GetRenderer());
	device = renderer->GetDevice();
}

VulkanShader::~VulkanShader()
{
	CleanUp();
}

unsigned int VulkanShader::GetId()
{
	return 0;
}

bool VulkanShader::AddShaderFromFile(const char * fileName, IShader::GLSLSHADERTYPE type)
{
	auto shaderCode = ReadFile(fileName);

	if (type == VERTEX)
	{
		vertexShaderModule = CreateShaderModule(shaderCode, device);

	}
	else if (type == FRAGMENT)
	{
		fragmentShaderModule = CreateShaderModule(shaderCode, device);
	}

	return true;
}

bool VulkanShader::Link()
{
	CreateDescriptorPool();
	CreateUniformBuffers();
	CreateDescriptorSetLayout();
	CreateDescriptorSets();

	CreateGraphicPipelines();

	vkDestroyShaderModule(device, fragmentShaderModule, nullptr);
	vkDestroyShaderModule(device, vertexShaderModule, nullptr);

	return true;
}

bool VulkanShader::IsLinked()
{
	return false;
}

void VulkanShader::CreateProgram(const std::string shaderName)
{
	name = shaderName;
}

void VulkanShader::SetUniform(const char * name, const float val)
{
}

void VulkanShader::SetUniform(const char * name, const int val)
{
}

unsigned int VulkanShader::GetUniformLocation(const char * name)
{
	return 0;
}

unsigned int VulkanShader::GetUniformLocation(std::string & name)
{
	return 0;
}

void VulkanShader::DrawMesh(MeshData & meshData)
{
}

void VulkanShader::DrawMesh(MeshData & meshData, VkCommandBuffer& buffer, int imageIndex, unsigned int stride)
{
	vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetPipeline(meshData.GetType()));

	// Draw the game to the texture buffer
	VkBuffer vertexBuffers[1];
	VkDeviceSize offsets[1];

	// Better to instance the mesh and change uniform locations
	vertexBuffers[0] = { meshData.vertexBuffer };
	offsets[0] = { 0 };

	// Bind buffers
	vkCmdBindVertexBuffers(buffer, 0, 1, vertexBuffers, offsets);

	if (meshData.bonesData.size() > 0)
	{
		VkBuffer vertexBuffers[1];
		VkDeviceSize offsets[1];
		vertexBuffers[0] = { meshData.boneBuffer };
		offsets[0] = { 0 };
		vkCmdBindVertexBuffers(buffer, 1, 1, vertexBuffers, offsets);
	}

	vkCmdBindIndexBuffer(buffer, meshData.indexBuffer, 0, VK_INDEX_TYPE_UINT32);

	BindDescriptorSet(meshData, buffer, imageIndex, stride);
	
	vkCmdDrawIndexed(buffer, static_cast<uint32_t>(meshData.GetIndices().size()), 1, 0, 0, 0);
}

VkPipeline VulkanShader::GetPipeline(MeshData::PrimativeType index)
{
	return graphicsPipeline[index];
}

VkPipelineLayout VulkanShader::GetPipelineLayout(MeshData::PrimativeType index)
{
	return pipelineLayout[index];
}

VkDescriptorSet& VulkanShader::GetDescriptorSet(unsigned int index)
{
	return descriptorSets[index];
}

VkDeviceSize VulkanShader::GetBufferSize()
{
	return bufferSize;
}

void VulkanShader::CreateUniformBuffers()
{
	auto renderer = static_cast<VulkanRenderer*>(BarnabusGameEngine::Get().GetRenderer());

	VkPhysicalDeviceProperties props;
	vkGetPhysicalDeviceProperties(renderer->GetPhysicalDevice(), &props);

	bufferSize = GetUniformItemSize();
	if (props.limits.minUniformBufferOffsetAlignment)
		bufferSize = (bufferSize + props.limits.minUniformBufferOffsetAlignment - 1) &
		~(props.limits.minUniformBufferOffsetAlignment - 1);

	uniformBuffers.resize(renderer->GetSwapChainImages().size());
	uniformBuffersMemory.resize(renderer->GetSwapChainImages().size());

	for (size_t i = 0; i < renderer->GetSwapChainImages().size(); i++)
	{
		VulkanUtils::CreateBuffer(renderer->GetDevice(), renderer->GetPhysicalDevice(), GetUniformBufferSize(), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

std::string VulkanShader::GetName()
{
	return name;
}

void VulkanShader::CreateGraphicPipelines()
{

	// Create vertex and fragment shader info.
	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertexShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragmentShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto bindingDescriptions = GetBindingDescription();
	auto attributeDescriptions = GetAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	auto renderer = static_cast<VulkanRenderer*>(BarnabusGameEngine::Get().GetRenderer());
	auto swapChainExtent = renderer->GetSwapChainExtent();

	// Viewport y and height are set to match coordinate system that OpenGL uses.
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = (float)swapChainExtent.height;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = -(float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	// Set rasterizer for culling & winding order.
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_NONE;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	// Manage depth and colour information.
	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	for (const auto& primative : primatives)
	{
		graphicsPipeline.insert(std::pair<MeshData::PrimativeType, VkPipeline>({ primative , VkPipeline()}));
		pipelineLayout.insert(std::pair<MeshData::PrimativeType, VkPipelineLayout>({ primative, VkPipelineLayout()}));

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.primitiveRestartEnable = VK_FALSE;
		switch (primative)
		{
		case MeshData::TRIANGLE:
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			break;
		case MeshData::LINE_STRIP:
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			break;
		case MeshData::QUAD:
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			break;
		default:
			break;
		}

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

		if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout[primative]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout!");
		}

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencil;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.layout = pipelineLayout[primative];
		pipelineInfo.renderPass = renderer->GetRenderPass();
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline[primative]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create graphics pipeline!");
		}
	}
}

void VulkanShader::Use(int index)
{
	uniformBufferIndex = index;
	Use();
}

void VulkanShader::CleanUp()
{
	for (auto const& x : graphicsPipeline)
	{
		vkDestroyPipeline(device, x.second, nullptr);
	}
	for (auto const& x : pipelineLayout)
	{
		vkDestroyPipelineLayout(device, x.second, nullptr);
	}
	for (size_t i = 0; i < uniformBuffers.size(); i++)
	{
		vkDestroyBuffer(device, uniformBuffers[i], nullptr);
		vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
	}

	vkDestroyDescriptorPool(device, descriptorPool, nullptr);
}

std::vector<VkVertexInputBindingDescription> VulkanShader::GetBindingDescription()
{
	std::vector<VkVertexInputBindingDescription> bindingDescription;
	bindingDescription.resize(1);
	
	bindingDescription[0].binding = 0;
	bindingDescription[0].stride = sizeof(Vertex);
	bindingDescription[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}

std::vector<VkVertexInputAttributeDescription> VulkanShader::GetAttributeDescriptions()
{
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
	attributeDescriptions.resize(4);

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, position);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, color);

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(Vertex, normal);

	attributeDescriptions[3].binding = 0;
	attributeDescriptions[3].location = 3;
	attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[3].offset = offsetof(Vertex, texCoords);

	return attributeDescriptions;
}