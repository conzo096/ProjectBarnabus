#pragma once
#include "IRenderer.h"

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
	GLFWwindow* window;
};