#pragma once
#define GLFW_INCLUDE_VULKAN
#include "IRenderer.h"
#include "GLFW/glfw3.h"
#include <optional>
#include <map>

class Light;
class UiQuad;
class VulkanShader;

struct BufferInfo
{
	VkBuffer vertexBuffer;
	VkBuffer indexBuffer;
	VkBuffer bonesBuffer;
	bool useBonesBuffer;
	VulkanShader* shader;
	size_t numIndices;
	MeshData::PrimativeType type;
};

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

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct Semaphores
	{
		// Swap chain image presentation
		VkSemaphore presentComplete;
		// Command buffer submission and execution
		VkSemaphore renderComplete;
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

public:
	// For rendering the game
	void Render() override;
	void SetCameraViewProjection(glm::mat4 camera) override;

	void AddMesh(MeshData& md) override;
	void AddLight(Light* light) override;
	glm::mat4 GetCameraVP() override;

	void AddUiElement(MeshData& md) override;
	void AddFramebuffer(std::pair<std::string, IFrameBuffer*> pair) override;
	IFrameBuffer* GetFrameBuffer(const std::string& buffer) override;

	virtual bool AddShader(std::string name, std::unique_ptr<IShader> shader) override;
	virtual IShader * GetShader(const std::string & shaderName) override;

public:
	// Vulkan specific
	VkExtent2D GetSwapChainExtent();
	VkSwapchainKHR GetSwapChain();
	VkDevice GetDevice();
	VkFormat GetSwapChainImageFormat();
	VkPhysicalDevice GetPhysicalDevice();
	std::vector<VkFramebuffer>& GetSwapChainFramebuffers();
	std::vector<VkImage>& GetSwapChainImages();
	VkQueue GetGraphicsQueue();
	VkQueue GetPresentQueue();
	VkCommandPool GetCommandPool();
	VkRenderPass GetRenderPass();

private:
	bool InitVulkanInstance();
	void SetupDebugMessenger();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateSurface();
	void CreateSwapChain();
	void CreateImageViews();
	void CreateDepthResources();

	void CreateRenderPass();
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateCommandBuffers();
	void CreateOffScreenCommandBuffer(std::vector<BufferInfo>& buffers, unsigned int imageIndex);
	void CreateSyncObjects();

private:
	void CleanupSwapChain();

	void RecreateSwapChain();
	void RecordCommandBuffer(unsigned int imageIndex);
public:
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
private:
	bool IsDeviceSuitable(VkPhysicalDevice device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
private:
	GLFWwindow* window;

	VkInstance instance;	
	VkDebugUtilsMessengerEXT debugMessenger;

	VkPhysicalDevice physicalDevice;
	VkDevice device;

	VkQueue graphicsQueue;
	VkQueue presentQueue;
	
	VkSurfaceKHR surface;

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	std::vector<VkImageView> swapChainImageViews;

	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	VkCommandBuffer offScreenCmdBuffer = VK_NULL_HANDLE;


	VkRenderPass renderPass;

	//Depth buffer - Add to framebuffer class?
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;


	// Semaphore used to synchronize between offscreen and final scene rendering
	VkSemaphore offscreenSemaphore = VK_NULL_HANDLE;

	Semaphores semaphores;

private:
	glm::mat4 cameraVP;

	// 
	std::map<VulkanShader*, std::vector<MeshData>> meshesToRender;
	std::map<std::string, std::vector<Light*>> environmentLights;
	
	std::vector<MeshData> uiElementsToRender;
	std::map<std::string, IFrameBuffer*> framebuffers;
	glm::vec4 backgroundColour;

	std::map<std::string, std::unique_ptr<IShader>> shaders;

	UiQuad* screenQuad;
};