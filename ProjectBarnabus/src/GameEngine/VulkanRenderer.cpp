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

	if (!InitVulkanInstance())
	{
		return false;
	}
	

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
	if (glewInit() != GLEW_OK)
	{
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

bool VulkanRenderer::InitVulkanInstance()
{
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	createInfo.enabledLayerCount = 0;

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		BarnabusGameEngine::Get().AddMessageLog(StringLog("Error: failed to create vulkan instance!", StringLog::Priority::Critical));
	}

	BarnabusGameEngine::Get().AddMessageLog(StringLog("Vulkan instance creation completed!", StringLog::Priority::Critical));
	return true;
}
