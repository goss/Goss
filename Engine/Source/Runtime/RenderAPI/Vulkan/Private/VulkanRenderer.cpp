#include "gepch.h"

#include "VulkanRenderer.h"
#include "Window.h"

#include <GLFW/glfw3.h>

namespace Goss
{
	VulkanRenderer::VulkanRenderer(Window& window, VulkanDevice& device) : window{window}, device{device}
	{
		RecreateSwapChain();
		CreateCommandBuffers();
	}

	VulkanRenderer::~VulkanRenderer() { FreeCommandBuffers(); }

	void VulkanRenderer::RecreateSwapChain()
	{
		VkExtent2D extent = GetExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = GetExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(device.GetDevice());

		if (swapChain == nullptr)
		{
			swapChain = std::make_unique<VulkanSwapChain>(device, extent);
		}
		else
		{
			std::shared_ptr oldSwapChain = std::move(swapChain);
			swapChain = std::make_unique<VulkanSwapChain>(device, extent, oldSwapChain);

			if (!oldSwapChain->CompareSwapFormats(*swapChain))
			{
				throw std::runtime_error("Swap chain image(or depth) format has changed!");
			}
		}
	}

	void VulkanRenderer::CreateCommandBuffers()
	{
		commandBuffers.resize(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = device.GetCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(device.GetDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate command buffers!");
		}
	}

	void VulkanRenderer::FreeCommandBuffers()
	{
		vkFreeCommandBuffers(
			device.GetDevice(),
			device.GetCommandPool(),
			static_cast<uint32_t>(commandBuffers.size()),
			commandBuffers.data());
		commandBuffers.clear();
	}

	VkCommandBuffer VulkanRenderer::BeginFrame()
	{
		const VkResult result = swapChain->AcquireNextImage(&currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		isFrameStarted = true;

		const VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}
		return commandBuffer;
	}

	void VulkanRenderer::EndFrame()
	{
		const VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}

		const VkResult result = swapChain->SubmitCommandBuffers(&commandBuffer, &currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)// || window.WasWindowResized())
		{
			//window.ResetResizedFlag();
			RecreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to present swap chain image!");
		}

		isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanRenderer::BeginSwapChainRenderPass(const VkCommandBuffer commandBuffer) const
	{
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = swapChain->GetRenderPass();
		renderPassInfo.framebuffer = swapChain->GetFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = swapChain->GetSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {{0.01f, 0.01f, 0.01f, 1.0f}};
		clearValues[1].depthStencil = {1.0f, 0};
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(swapChain->GetSwapChainExtent().width);
		viewport.height = static_cast<float>(swapChain->GetSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		const VkRect2D scissor{{0, 0}, swapChain->GetSwapChainExtent()};
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void VulkanRenderer::EndSwapChainRenderPass(const VkCommandBuffer commandBuffer) const
	{
		vkCmdEndRenderPass(commandBuffer);
	}
}
