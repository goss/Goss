#pragma once
#include "ApplicationWindow.h"
#include "SwapChain.h"

// std
#include <cassert>
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Goss
{
	class Renderer
	{
	public:
		Renderer(ApplicationWindow& window, Device& device);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		VkRenderPass GetSwapChainRenderPass() const { return swapChain->GetRenderPass(); }
		bool IsFrameInProgress() const { return isFrameStarted; }

		VkCommandBuffer GetCurrentCommandBuffer() const
		{
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex];
		}

		int GetFrameIndex() const
		{
			assert(isFrameStarted && "Cannot get frame index when frame not in progress");
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

		ApplicationWindow& window;
		Device& device;
		std::unique_ptr<SwapChain> swapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex;
		bool isFrameStarted;
	};
}
