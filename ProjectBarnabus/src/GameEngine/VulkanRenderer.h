#pragma once
#define GLFW_INCLUDE_VULKAN
#include "IRenderer.h"
#include "GLFW/glfw3.h"
#include <optional>

class VulkanRenderer : public IRenderer
{
public:
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

public:
	VulkanRenderer();
	~VulkanRenderer();

public:
	bool InitialiseGameEngine() override;
	bool ShouldWindowClose() override;
	GLFWwindow* GetWindow() override;

	GraphicsRenderer GetRenderType() override;


public:
	// For meshes
	void InitialiseMesh(MeshData& data) override;
	void UpdateBaseVertexBuffers(MeshData& data) override;
private:
	bool InitVulkanInstance();
	void SetupDebugMessenger();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateSurface();

private:
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	bool IsDeviceSuitable(VkPhysicalDevice device);
private:
	GLFWwindow* window;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;
};