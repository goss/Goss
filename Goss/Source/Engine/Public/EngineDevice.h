#pragma once
#include "ApplicationWindow.h"

// std
#include <vector>

namespace Goss
{
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities{};
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct QueueFamilyIndices
	{
		uint32_t graphicsFamily{};
		uint32_t presentFamily{};
		bool graphicsFamilyHasValue = false;
		bool presentFamilyHasValue = false;
		bool IsComplete() const { return graphicsFamilyHasValue && presentFamilyHasValue; }
	};

	class EngineDevice
	{
	public:
#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		explicit EngineDevice(ApplicationWindow& window);
		~EngineDevice();

		// Not copyable or movable
		EngineDevice(const EngineDevice&) = delete;
		EngineDevice& operator=(const EngineDevice&) = delete;

		VkCommandPool GetCommandPool() const { return commandPool; }
		VkDevice GetDevice() const { return vkDevice; }
		VkSurfaceKHR Surface() const { return vkSurfaceKHR; }
		VkQueue GraphicsQueue() const { return graphicsQueue; }
		VkQueue PresentQueue() const { return presentQueue; }

		SwapChainSupportDetails GetSwapChainSupport() const { return QuerySwapChainSupport(physicalDevice); }
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags flags) const;
		QueueFamilyIndices FindPhysicalQueueFamilies() const { return FindQueueFamilies(physicalDevice); }
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

		// Buffer Helper Functions
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags flags, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;

		VkCommandBuffer BeginSingleTimeCommands() const;
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer) const;
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount) const;
		void CreateImageWithInfo(const VkImageCreateInfo& imageInfo, VkMemoryPropertyFlags flags, VkImage& image, VkDeviceMemory& imageMemory) const;

		VkPhysicalDeviceProperties properties{};

	private:
		void CreateInstance();
		void SetupDebugMessenger();
		void CreateSurface();
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		void CreateCommandPool();

		// Helper functions
		bool IsDeviceSuitable(VkPhysicalDevice device) const;
		std::vector<const char*> GetRequiredExtensions() const;
		bool CheckValidationLayerSupport() const;
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;
		static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void HasGFLWRequiredInstanceExtensions() const;
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device) const;
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;

		ApplicationWindow& window;

		VkInstance instance{};
		VkDebugUtilsMessengerEXT debugMessenger{};
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkCommandPool commandPool{};
		VkDevice vkDevice{};
		VkSurfaceKHR vkSurfaceKHR{};
		VkQueue graphicsQueue{};
		VkQueue presentQueue{};

		const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
		const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	};
}
