#pragma once
#include "VulkanSwapChain.h"

#include <vulkan/vulkan_core.h>

#include "Window.h"

namespace Goss
{
	class VulkanRenderer
	{
	public:
		explicit VulkanRenderer(Window& window, VulkanDevice& device);
		~VulkanRenderer();

		VulkanRenderer(const VulkanRenderer&) = delete;
		VulkanRenderer& operator=(const VulkanRenderer&) = delete;

		VkRenderPass GetSwapChainRenderPass() const { return swapChain->GetRenderPass(); }
		bool IsFrameInProgress() const { return isFrameStarted; }
		float GetAspectRatio() const { return swapChain->ExtentAspectRatio(); }

		VkCommandBuffer GetCurrentCommandBuffer() const
		{
			return commandBuffers[currentFrameIndex];
		}

		int GetFrameIndex() const
		{
			return currentFrameIndex;
		}

		VkCommandBuffer BeginFrame();
		void EndFrame();

		void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer) const;
		void EndSwapChainRenderPass(VkCommandBuffer commandBuffer) const;

	private:
		void CreateCommandBuffers();
		void FreeCommandBuffers();
		void RecreateSwapChain();

		VkExtent2D GetExtent() const { return {(window.GetWidth()), (window.GetHeight())}; }

		Window& window;
		VulkanDevice& device;
		std::unique_ptr<VulkanSwapChain> swapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex = 0;
		int currentFrameIndex = 0;
		bool isFrameStarted;
	};
}
