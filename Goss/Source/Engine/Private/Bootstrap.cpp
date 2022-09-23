#include "Bootstrap.h"

// std
#include <array>
#include <stdexcept>

namespace Goss
{
	Bootstrap::Bootstrap()
	{
		CreatePipelineLayout();
		CreatePipeline();
		CreateCommandBuffers();
	}

	Bootstrap::~Bootstrap()
	{
		vkDestroyPipelineLayout(engineDevice.Device(), pipelineLayout, nullptr);
	}

	void Bootstrap::Run()
	{
		while (!appWindow.ShouldClose())
		{
			glfwPollEvents();
			DrawFrame();
		}

		vkDeviceWaitIdle(engineDevice.Device());
	}

	void Bootstrap::CreatePipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(engineDevice.Device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout!");
		}
	}

	void Bootstrap::CreatePipeline()
	{
		PipelineConfigInfo pipelineConfig = Pipeline::DefaultPipelineConfigInfo(swapChain.Width(), swapChain.Height());
		pipelineConfig.renderPass = swapChain.GetRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		pipeline = std::make_unique<Pipeline>(engineDevice,
			"shaders/simple_shader.vert.spv",
			"shaders/simple_shader.frag.spv",
			pipelineConfig);
	}

	void Bootstrap::CreateCommandBuffers()
	{
		commandBuffers.resize(swapChain.ImageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = engineDevice.GetCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(engineDevice.Device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffers!");
		}

		const int size = static_cast<int>(commandBuffers.size());
		for (int i = 0; i < size; i++)
		{
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to begin recording command buffer!");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = swapChain.GetRenderPass();
			renderPassInfo.framebuffer = swapChain.GetFrameBuffer(i);

			renderPassInfo.renderArea.offset = {0, 0};
			renderPassInfo.renderArea.extent = swapChain.GetSwapChainExtent();

			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = {{0.1f, 0.1f, 0.1f, 1.0f}};
			clearValues[1].depthStencil = {1.0f, 0};
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			pipeline->Bind(commandBuffers[i]);
			vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(commandBuffers[i]);
			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to record command buffer!");
			}
		}
	}

	void Bootstrap::DrawFrame()
	{
		uint32_t imageIndex;
		VkResult result = swapChain.AcquireNextImage(&imageIndex);
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire swap chain image!");
		}

		result = swapChain.SubmitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to present swap chain image!");
		}
	}
}
