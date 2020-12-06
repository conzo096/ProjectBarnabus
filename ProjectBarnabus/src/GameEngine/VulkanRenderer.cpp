#define GLFW_INCLUDE_VULKAN
#include "VulkanRenderer.h"
#include "BarnabusGameEngine.h"

#include "VulkanUtils.h"
#include "VkFinalPassShader.h"

#include "UiQuad.h"

#include <algorithm>
#include <set>
#include <array>
namespace
{
#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

	const std::vector<const char*> validationLayers =
	{
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> deviceExtensions =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
	{
		std::cerr << "validation layer: " << callbackData->pMessage << std::endl;
		std::string errorMessage = "validation layer:";
		errorMessage += callbackData->pMessage;

		BarnabusGameEngine::Get().AddMessageLog(StringLog(errorMessage, StringLog::Priority::Critical));
		return VK_FALSE;
	}

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}

	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
	}

	std::vector<const char*> GetRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool CheckValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}
		}

		return true;
	}

	bool CheckDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
		{
			return capabilities.currentExtent;
		}
		else
		{
			VkExtent2D actualExtent = { 1920, 1080 };

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}
} //namespace

VulkanRenderer::VulkanRenderer() : physicalDevice(VK_NULL_HANDLE)
{
}

VulkanRenderer::~VulkanRenderer()
{
	for (auto& shader : shaders)
	{
		shader.second->~IShader();
	}

	vkDestroySemaphore(device, semaphores.presentComplete, nullptr);
	vkDestroySemaphore(device, semaphores.renderComplete, nullptr);
	vkDestroySemaphore(device, offscreenSemaphore, nullptr);

	for (auto framebuffer : swapChainFramebuffers)
	{
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}

	for (auto imageView : swapChainImageViews)
	{
		vkDestroyImageView(device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(device, swapChain, nullptr);
	vkDestroyCommandPool(device, commandPool, nullptr);

	vkDestroyDevice(device, nullptr);

	if (enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);
}

bool VulkanRenderer::InitialiseGameEngine()
{
	if (!glfwInit())
	{
		BarnabusGameEngine::Get().AddMessageLog(StringLog("ERROR: glfw failed init! exiting.", StringLog::Priority::Critical));
		return false;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(1920, 1080, "Testing - Vulkan Renderer", NULL, NULL);
	if (!window)
	{
		assert(window != NULL);
		BarnabusGameEngine::Get().AddMessageLog(StringLog("Error: Window could not be initialised!", StringLog::Priority::Critical));
		return false;
	}

	// V-Sync, does not run without it
	glfwSwapInterval(1);

	// Window is now initalised, now make it the current context.
	glfwMakeContextCurrent(window);

	if (!InitVulkanInstance())
	{
		return false;
	}

	SetupDebugMessenger();

	CreateSurface();
	PickPhysicalDevice();
	CreateLogicalDevice();
	CreateSwapChain();
	CreateRenderPass();
	CreateImageViews();
	CreateCommandPool();
	CreateDepthResources();
	PrepareOffscreenFramebuffer();
	CreateFramebuffers();

	CreateSyncObjects();

	screenQuad = new UiQuad(glm::vec2(-1, -1), glm::vec2(1, 1));
	screenQuad->InitQuad();

	// Add final shader
	auto finalShader = std::make_unique<VkFinalPassShader>();
	finalShader->CreateProgram("FinalRender");
	finalShader->AddShaderFromFile("res\\shaders\\vulkan\\VkFinalPassVert.spv", GLShader::VERTEX);
	finalShader->AddShaderFromFile("res\\shaders\\vulkan\\VkFinalPassFrag.spv", GLShader::FRAGMENT);
	finalShader->Link();
	AddShader("final", std::move(finalShader));

	CreateCommandBuffers();

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

VkExtent2D VulkanRenderer::GetSwapChainExtent()
{
	return swapChainExtent;
}

VkSwapchainKHR VulkanRenderer::GetSwapChain()
{
	return swapChain;
}

VkDevice VulkanRenderer::GetDevice()
{
	return device;
}

VkFormat VulkanRenderer::GetSwapChainImageFormat()
{
	return swapChainImageFormat;
}

VkPhysicalDevice VulkanRenderer::GetPhysicalDevice()
{
	return physicalDevice;
}

std::vector<VkFramebuffer>& VulkanRenderer::GetSwapChainFramebuffers()
{
	return swapChainFramebuffers;
}

std::vector<VkImage>& VulkanRenderer::GetSwapChainImages()
{
	return swapChainImages;
}

VkQueue VulkanRenderer::GetGraphicsQueue()
{
	return graphicsQueue;
}

VkQueue VulkanRenderer::GetPresentQueue()
{
	return presentQueue;
}

VkCommandPool VulkanRenderer::GetCommandPool()
{
	return commandPool;
}

VkRenderPass VulkanRenderer::GetRenderPass()
{
	return mainFramebuffer.GetRenderPass();
}

VkRenderPass VulkanRenderer::GetOffScreenRenderPass()
{
	return offScreenFrameBuf.GetRenderPass();
}

VulkanFrameBuffer VulkanRenderer::GetOffscreenFrameBuffer()
{
	return offScreenFrameBuf;
}

bool VulkanRenderer::InitVulkanInstance()
{
	if (enableValidationLayers && !CheckValidationLayerSupport())
	{
		BarnabusGameEngine::Get().AddMessageLog(StringLog("Error: validation layers requested, but not available!", StringLog::Priority::Critical));
		throw std::runtime_error("validation layers requested, but not available!");
	}

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

	auto extensions = GetRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
	{
		BarnabusGameEngine::Get().AddMessageLog(StringLog("Error: failed to create vulkan instance!", StringLog::Priority::Critical));
		return false;
	}

	BarnabusGameEngine::Get().AddMessageLog(StringLog("Vulkan instance creation completed!", StringLog::Priority::Critical));
	return true;
}

void VulkanRenderer::SetupDebugMessenger()
{
	if (!enableValidationLayers)
	{
		return;
	}

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	PopulateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
	{
		BarnabusGameEngine::Get().AddMessageLog(StringLog("Error: failed to set up vulkan debug messenger!", StringLog::Priority::Critical));
	}
}

void VulkanRenderer::PickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		BarnabusGameEngine::Get().AddMessageLog(StringLog("Error: failed to find GPUs with Vulkan support!", StringLog::Priority::Critical));
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices)
	{
		if (IsDeviceSuitable(device))
		{
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE)
	{
		BarnabusGameEngine::Get().AddMessageLog(StringLog("Error: failed to find a suitable GPU!", StringLog::Priority::Critical));
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

void VulkanRenderer::CreateLogicalDevice()
{
	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}
	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
	{
		BarnabusGameEngine::Get().AddMessageLog(StringLog("Error: failed to create logical device!", StringLog::Priority::Critical));
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void VulkanRenderer::CreateSurface()
{
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
	{
		BarnabusGameEngine::Get().AddMessageLog(StringLog("Error: failed to create window surface!", StringLog::Priority::Critical));
		throw std::runtime_error("failed to create window surface!");
	}
}

void VulkanRenderer::CreateSwapChain()
{
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

void VulkanRenderer::CreateImageViews()
{
	swapChainImageViews.resize(swapChainImages.size());

	for (uint32_t i = 0; i < swapChainImages.size(); i++)
	{
		swapChainImageViews[i] = VulkanUtils::CreateImageView(device,swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}

}

void VulkanRenderer::CreateRenderPass()
{
	mainFramebuffer.CreateRenderPass(device, physicalDevice, swapChainImageFormat);
	offScreenFrameBuf.CreateRenderPass(device, physicalDevice, swapChainImageFormat);
}

void VulkanRenderer::CreateFramebuffers()
{
	for (auto framebuffer : swapChainFramebuffers)
	{
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}

	swapChainFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); i++)
	{
		std::array<VkImageView, 2> attachments = {
			swapChainImageViews[i],
			static_cast<VulkanTexture*>(mainFramebuffer.GetDepthTexture())->GetImageView()
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = mainFramebuffer.GetRenderPass();
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create framebuffer!");
		}
	}

	offScreenFrameBuf.CreateFrameBuffer(device);
}

void VulkanRenderer::CreateCommandPool()
{
	VulkanRenderer::QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalDevice);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}
}

// Create the buffer that uses the UI and game buffers & displays to screen.
void VulkanRenderer::CreateCommandBuffers()
{
	commandBuffers.resize(swapChainFramebuffers.size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}

	VkCommandBufferBeginInfo beginInfo{};
	VkRenderPassBeginInfo renderPassInfo{};
	std::array<VkClearValue, 2> clearValues{};

	for (size_t i = 0; i < commandBuffers.size(); i++)
	{
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = mainFramebuffer.GetRenderPass();
		renderPassInfo.framebuffer = swapChainFramebuffers[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChainExtent;

		clearValues[0].color = { 0.1f, 0.0f, 0.4f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = 2;
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		// Draw to screen - passing in both texture buffers
		VkFinalPassShader* shader = static_cast<VkFinalPassShader*>(shaders["final"].get());

		uint32_t uniformOffset[1] = { 0 };

		vkCmdBindDescriptorSets(commandBuffers[i],
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			shader->GetPipelineLayout(MeshData::PrimativeType::QUAD),
			0,
			1,
			&shader->GetDescriptorSet(i),
			1,
			uniformOffset);

		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipeline(screenQuad->GetMeshData().GetType()));

		VkBuffer vertexBuffers[] = { screenQuad->GetMeshData().vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffers[i], screenQuad->GetMeshData().indexBuffer, 0, VK_INDEX_TYPE_UINT32);

		vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(screenQuad->GetMeshData().GetIndices().size()), 1, 0, 0, 0);

		vkCmdEndRenderPass(commandBuffers[i]);

		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}

void VulkanRenderer::CreateSyncObjects()
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphores.presentComplete);
	vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphores.renderComplete);

	vkCreateSemaphore(device, &semaphoreInfo, nullptr, &offscreenSemaphore);
}

void VulkanRenderer::CleanupSwapChain()
{
	vkDestroyImageView(device, static_cast<VulkanTexture*>(mainFramebuffer.GetDepthTexture())->GetImageView() , nullptr);
	vkDestroyImage(device, static_cast<VulkanTexture*>(mainFramebuffer.GetDepthTexture())->GetImage(), nullptr);
	vkFreeMemory(device, static_cast<VulkanTexture*>(mainFramebuffer.GetDepthTexture())->GetImageMemory(), nullptr);

	for (const auto& framebuffer : swapChainFramebuffers)
	{
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}

	vkDestroyRenderPass(device, mainFramebuffer.GetRenderPass(), nullptr);

	vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

	for (auto imageView : swapChainImageViews)
	{
		vkDestroyImageView(device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(device, swapChain, nullptr);

	for (auto& shader : shaders)
	{
		static_cast<VulkanShader*>(shader.second.get())->CleanUp();
	}
}

void VulkanRenderer::RecreateSwapChain()
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device);

	CleanupSwapChain();

	CreateSwapChain();
	CreateImageViews();
	CreateRenderPass();

	for (auto& shader : shaders)
	{
		static_cast<VulkanShader*>(shader.second.get())->CreateGraphicPipelines();
	}

	CreateDepthResources();
	CreateFramebuffers();

	for (auto& shader : shaders)
	{
		static_cast<VulkanShader*>(shader.second.get())->CreateUniformBuffers();
		static_cast<VulkanShader*>(shader.second.get())->CreateDescriptorPool();
		static_cast<VulkanShader*>(shader.second.get())->CreateDescriptorSets();
	}
}

VulkanRenderer::QueueFamilyIndices VulkanRenderer::FindQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.isComplete())
		{
			break;
		}

		i++;
	}

	return indices;
}

bool VulkanRenderer::IsDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = FindQueueFamilies(device);

	bool extensionsSupported = CheckDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported)
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

VulkanRenderer::SwapChainSupportDetails VulkanRenderer::QuerySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

IRenderer::GraphicsRenderer VulkanRenderer::GetRenderType()
{
	return IRenderer::Vulkan;
}


void VulkanRenderer::InitialiseMesh(MeshData& data)
{
	data.CreateVertexBuffer(data.vertexBuffer, data.vertexBufferMemory, commandPool);
	data.CreateIndexBuffer(data.indexBuffer, data.indexBufferMemory, commandPool);

	// If data has animation data.
	if (data.GetBoneData().size() > 0)
	{
		data.CreateBoneBuffer(data.boneBuffer, data.boneBufferMemory, commandPool);
	}
}

void VulkanRenderer::UpdateBaseVertexBuffers(MeshData& data)
{
	//data.CreateVertexBuffer(data.vertexBuffer, data.vertexBufferMemory, commandPool);
}

void VulkanRenderer::CreateOffScreenCommandBuffer(unsigned int imageIndex)
{
	vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(1), &offScreenCmdBuffer);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(device, &allocInfo, &offScreenCmdBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}

	// Record command buffer

	VkCommandBufferBeginInfo beginInfo{};
	VkRenderPassBeginInfo renderPassInfo{};

	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = offScreenFrameBuf.GetRenderPass();
	renderPassInfo.framebuffer = offScreenFrameBuf.GetVulkanFrameBuffer();
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;
	std::array<VkClearValue, 2> clearValues{};

	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(offScreenCmdBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	clearValues[0].color = { 0.1f, 0.0f, 0.4f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = 2;
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(offScreenCmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	unsigned int stride = 0;
	for (auto& meshes : meshesToRender)
	{
		// Tell Shader here to use shader.
		meshes.first->Use(imageIndex);
	
		for (auto& mesh : meshes.second)
		{
			meshes.first->UpdateUniforms(mesh);
			meshes.first->DrawMesh(mesh, offScreenCmdBuffer, imageIndex, stride);
			stride++;
		}
		stride = 0;
	}

	// Draw ui elements now
	VkBuffer vertexBuffers[1];
	VkDeviceSize offsets[1];
	for (int i = 0; i < uiElementsToRender.size(); i++)
	{
		vkCmdBindPipeline(offScreenCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, static_cast<VulkanShader*>(uiElementsToRender[i].GetShader())->GetPipeline(MeshData::PrimativeType::QUAD));
		vertexBuffers[0] = { uiElementsToRender[i].vertexBuffer };
		offsets[0] = { 0 };
		uint32_t uniformOffset[1] = { 0 };
		vkCmdBindVertexBuffers(offScreenCmdBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindDescriptorSets(offScreenCmdBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			static_cast<VulkanShader*>(uiElementsToRender[i].GetShader())->GetPipelineLayout(MeshData::PrimativeType::QUAD)
			, 0,
			1,
			&static_cast<VulkanShader*>(uiElementsToRender[i].GetShader())->GetDescriptorSet(imageIndex),
			1,
			uniformOffset);
	
		vkCmdDrawIndexed(offScreenCmdBuffer, static_cast<uint32_t>(uiElementsToRender[i].GetIndices().size()), 1, 0, 0, 0);
	}

	vkCmdEndRenderPass(offScreenCmdBuffer);

	vkEndCommandBuffer(offScreenCmdBuffer);
}

void VulkanRenderer::Render()
{
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, semaphores.presentComplete, VK_NULL_HANDLE, &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR )
	{
		RecreateSwapChain();
		return;
	}

	CreateOffScreenCommandBuffer(imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.commandBufferCount = 1;

	// OffScreen rendering

	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.pWaitSemaphores = &semaphores.presentComplete;
	submitInfo.pSignalSemaphores = &offscreenSemaphore;
	submitInfo.pCommandBuffers = &offScreenCmdBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

	// Now draw to screen.

	submitInfo.pWaitSemaphores = &offscreenSemaphore;
	submitInfo.pSignalSemaphores = &semaphores.renderComplete;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

	// Put this in a swapchain class?
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = NULL;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &semaphores.renderComplete;

	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapChain;

	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(presentQueue, &presentInfo);

	vkQueueWaitIdle(presentQueue);

	meshesToRender.clear();
	uiElementsToRender.clear();
	environmentLights.clear();
	glfwSwapBuffers(window);
}

void VulkanRenderer::SetCameraViewProjection(glm::mat4 camera)
{
	cameraVP = camera;
}

void VulkanRenderer::AddMesh(MeshData & md)
{
	VulkanShader* shader = dynamic_cast<VulkanShader*>(md.GetShader());

	if (shader)
	{
		auto environmentMeshes = meshesToRender.find(shader);

		// enviroment does not exist. Add a new vector to list
		if (environmentMeshes == meshesToRender.end())
		{
			std::vector<MeshData> newList;
			newList.push_back(md);
			meshesToRender.insert(std::pair<VulkanShader*, std::vector<MeshData>>(shader, newList));
		}
		else
		{
			environmentMeshes->second.push_back(md);
		}
	}
}

void VulkanRenderer::AddLight(Light * light)
{
}

glm::mat4 VulkanRenderer::GetCameraVP()
{
	return cameraVP;
}

void VulkanRenderer::AddUiElement(MeshData & md)
{
	uiElementsToRender.push_back(md);
}

void VulkanRenderer::AddFramebuffer(std::pair<std::string, IFrameBuffer*> pair)
{
}

IFrameBuffer* VulkanRenderer::GetFrameBuffer(const std::string& buffer)
{
	return framebuffers.at(buffer);
}

bool VulkanRenderer::AddShader(std::string name, std::unique_ptr<IShader> shader)
{
	auto result = shaders.insert(std::pair < std::string, std::unique_ptr<IShader>>(name, std::move(shader)));
	return result.second;
}

IShader * VulkanRenderer::GetShader(const std::string & shaderName)
{
	auto it = shaders.find(shaderName);
	assert(it != shaders.end());

	return it->second.get();
}

void VulkanRenderer::CreateDepthResources()
{
	VkFormat depthFormat = VulkanUtils::FindDepthFormat(physicalDevice);

	VulkanUtils::CreateImage(device, physicalDevice, swapChainExtent.width, swapChainExtent.height,
		depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
		static_cast<VulkanTexture*>(mainFramebuffer.GetDepthTexture())->GetImage(), static_cast<VulkanTexture*>(mainFramebuffer.GetDepthTexture())->GetImageMemory());
	static_cast<VulkanTexture*>(mainFramebuffer.GetDepthTexture())->SetImageView(VulkanUtils::CreateImageView(device, static_cast<VulkanTexture*>(mainFramebuffer.GetDepthTexture())->GetImage(), depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT));
}

//todo move this into the texture class
void VulkanRenderer::CreateAttachement(VkFormat format, VkImageUsageFlagBits usage, VulkanTexture * attachment)
{
	VkImageAspectFlags aspectMask = 0;
	VkImageLayout imageLayout;

	attachment->SetFormat(format);

	if (usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
	{
		aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	}
	if (usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
	{
		aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	}

	assert(aspectMask > 0);

	VkImageCreateInfo image{};
	image.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image.imageType = VK_IMAGE_TYPE_2D;
	image.format = format;
	image.extent.width = offScreenFrameBuf.GetWidth();
	image.extent.height = offScreenFrameBuf.GetHeight();
	image.extent.depth = 1;
	image.mipLevels = 1;
	image.arrayLayers = 1;
	image.samples = VK_SAMPLE_COUNT_1_BIT;
	image.tiling = VK_IMAGE_TILING_OPTIMAL;
	image.usage = usage | VK_IMAGE_USAGE_SAMPLED_BIT;

	VkMemoryAllocateInfo memAlloc{};
	memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	VkMemoryRequirements memReqs;

	vkCreateImage(device, &image, nullptr, &attachment->GetImage());
	vkGetImageMemoryRequirements(device, attachment->GetImage(), &memReqs);
	memAlloc.allocationSize = memReqs.size;
	memAlloc.memoryTypeIndex = VulkanUtils::FindMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, physicalDevice);
	vkAllocateMemory(device, &memAlloc, nullptr, &attachment->GetImageMemory());
	vkBindImageMemory(device, attachment->GetImage(), attachment->GetImageMemory(), 0);

	VkImageViewCreateInfo imageView{};
	imageView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageView.format = format;
	imageView.subresourceRange = {};
	imageView.subresourceRange.aspectMask = aspectMask;
	imageView.subresourceRange.baseMipLevel = 0;
	imageView.subresourceRange.levelCount = 1;
	imageView.subresourceRange.baseArrayLayer = 0;
	imageView.subresourceRange.layerCount = 1;
	imageView.image = attachment->GetImage();

	vkCreateImageView(device, &imageView, nullptr, &attachment->GetImageView());
}

void VulkanRenderer::PrepareOffscreenFramebuffer()
{
	offScreenFrameBuf.LoadFrameBuffer(1920, 1080);

	// Get textures
	auto frameTexture = static_cast<VulkanTexture*>(offScreenFrameBuf.GetFrameTexture());
	auto depthTexture = static_cast<VulkanTexture*>(offScreenFrameBuf.GetDepthTexture());

	// Albedo (color)
	CreateAttachement(
		VK_FORMAT_B8G8R8A8_SRGB,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		frameTexture);

	// Depth attachment

	// Find a suitable depth format
	VkFormat attDepthFormat = VulkanUtils::FindDepthFormat(physicalDevice);

	CreateAttachement(
		attDepthFormat,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		depthTexture);
}