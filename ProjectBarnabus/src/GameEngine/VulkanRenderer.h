#define GLFW_INCLUDE_VULKAN
#pragma once
#include "IRenderer.h"
#include "GLFW/glfw3.h"

class VulkanRenderer : public IRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

public:
	bool InitialiseGameEngine() override;
	bool ShouldWindowClose() override;
	GLFWwindow* GetWindow() override;

private:
	bool InitVulkanInstance();
private:
	GLFWwindow* window;
	VkInstance instance;
};