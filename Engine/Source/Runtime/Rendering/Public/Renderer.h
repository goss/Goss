#pragma once
#include "Window.h"
#include "SwapChain.h"

#include <vulkan/vulkan_core.h>

namespace Goss
{
	class Renderer
	{
	public:
		Renderer(Window& window, Device& device);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

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

		Window& window;
		Device& device;
		std::unique_ptr<SwapChain> swapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex = 0;
		int currentFrameIndex = 0;
		bool isFrameStarted;
	};
}
