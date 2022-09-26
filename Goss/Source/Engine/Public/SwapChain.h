#pragma once
#include "EngineDevice.h"

// std
#include <vector>

namespace Goss
{
	class SwapChain
	{
	public:
		static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

		SwapChain(EngineDevice& deviceRef, VkExtent2D extent);
		~SwapChain();

		SwapChain(const SwapChain&) = delete;
		SwapChain& operator=(const SwapChain&) = delete;

		VkFramebuffer GetFrameBuffer(const int index) const { return swapChainFrameBuffers[index]; }
		VkRenderPass GetRenderPass() const { return renderPass; }
		VkImageView GetImageView(const int index) const { return swapChainImageViews[index]; }
		size_t ImageCount() const { return swapChainImages.size(); }
		VkFormat GetSwapChainImageFormat() const { return swapChainImageFormat; }
		VkExtent2D GetSwapChainExtent() const { return swapChainExtent; }
		uint32_t Width() const { return swapChainExtent.width; }
		uint32_t Height() const { return swapChainExtent.height; }

		float ExtentAspectRatio() const
		{
			return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
		}

		VkFormat FindDepthFormat() const;

		VkResult AcquireNextImage(uint32_t* imageIndex) const;
		VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, const uint32_t* imageIndex);

	private:
		void CreateSwapChain();
		void CreateImageViews();
		void CreateDepthResources();
		void CreateRenderPass();
		void CreateFrameBuffers();
		void CreateSyncObjects();

		// Helper functions
		static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent{};

		std::vector<VkFramebuffer> swapChainFrameBuffers;
		VkRenderPass renderPass{};

		std::vector<VkImage> depthImages;
		std::vector<VkDeviceMemory> depthImageMemory;
		std::vector<VkImageView> depthImageViews;
		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;

		EngineDevice& device;
		VkExtent2D windowExtent;

		VkSwapchainKHR swapChain{};

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
		size_t currentFrame = 0;
	};
}
