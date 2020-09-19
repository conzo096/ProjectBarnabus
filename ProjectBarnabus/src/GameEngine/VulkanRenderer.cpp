#define GLFW_INCLUDE_VULKAN
#include "VulkanRenderer.h"
#include "BarnabusGameEngine.h"

VulkanRenderer::VulkanRenderer()
{
}

VulkanRenderer::~VulkanRenderer()
{
}

bool VulkanRenderer::InitialiseGameEngine()
{
	if (!glfwInit())
	{
		BarnabusGameEngine::Get().AddMessageLog(StringLog("ERROR: glfw failed init! exiting.", StringLog::Priority::Critical));
		return false;
	}

	window = glfwCreateWindow(1920, 1080, "Testing - Vulkan Renderer", NULL, NULL);

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::string extensionLog(std::to_string(extensionCount) + " extensions supported for Vulkan");
	BarnabusGameEngine::Get().AddMessageLog(StringLog(extensionLog, StringLog::Priority::Critical));

	// Window is now initalised, now make it the current context.
	glfwMakeContextCurrent(window);

	if (!window)
	{
		assert(window != NULL);
		BarnabusGameEngine::Get().AddMessageLog(StringLog("Error: Window could not be initialised!", StringLog::Priority::Critical));
		return false;
	}

	// Set up glew.
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		BarnabusGameEngine::Get().AddMessageLog(StringLog("ERROR: glew failed init!exiting.", StringLog::Priority::Critical));
		return false;
	}
	// glExperimental throws junk errors, Ignore.
	glGetError();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// V-Sync, does not run without it
	glfwSwapInterval(1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

bool VulkanRenderer::ShouldWindowClose()
{
	return glfwWindowShouldClose(window);
}

GLFWwindow * VulkanRenderer::GetWindow()
{
	return window;
}
