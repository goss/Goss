#include "Bootstrap.h"

// glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <ctime>
#include <stdexcept>

#include "GameObject.h"

namespace Goss
{
	/**
	 * Limited Constant data size is 128 bytes
	 * Used for instancing
	 */
	struct ConstantData
	{
		glm::mat2 transform{1.0f};
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

	Bootstrap::Bootstrap()
	{
		LoadGameObjects();
		CreatePipelineLayout();
		CreateSwapChain();
		CreateCommandBuffers();
	}

	Bootstrap::~Bootstrap()
	{
		vkDestroyPipelineLayout(engineDevice.GetDevice(), pipelineLayout, nullptr);
	}

	void Bootstrap::Run()
	{
		while (!appWindow.ShouldClose())
		{
			glfwPollEvents();
			DrawFrame();
		}

		vkDeviceWaitIdle(engineDevice.GetDevice());
	}

	void Bootstrap::LoadGameObjects()
	{
		std::vector<Model::Vertex> vertices
		{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		};
		//Sierpinski(vertices, 3, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});

		GameObject triangle = GameObject::CreateGameObject();
		triangle.model = std::make_unique<Model>(engineDevice, vertices);
		triangle.color = {.1f, .8f, .1f};
		triangle.transform.translation.x = .2f;
		triangle.transform.scale = {2.f, .5f};
		triangle.transform.rotation = .25f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));
	}

	void Bootstrap::Sierpinski(std::vector<Model::Vertex>& vertices, const int depth, const glm::vec2 left, const glm::vec2 right, const glm::vec2 top)
	{
		if (depth <= 0)
		{
			vertices.push_back({top});
			vertices.push_back({right});
			vertices.push_back({left});
		}
		else
		{
			const auto leftTop = 0.5f * (left + top);
			const auto rightTop = 0.5f * (right + top);
			const auto leftRight = 0.5f * (left + right);
			Sierpinski(vertices, depth - 1, left, leftRight, leftTop);
			Sierpinski(vertices, depth - 1, leftRight, right, rightTop);
			Sierpinski(vertices, depth - 1, leftTop, rightTop, top);
		}
	}

	void Bootstrap::CreatePipelineLayout()
	{
		VkPushConstantRange pushConstantRange;
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(ConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(engineDevice.GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout!");
		}
	}

	void Bootstrap::CreatePipeline()
	{
		assert(swapChain != VK_NULL_HANDLE && "Cannot create pipeline before swap chain");
		assert(pipelineLayout != VK_NULL_HANDLE && "Cannot create pipeline before layout");

		PipelineConfigInfo pipelineConfig{};
		Pipeline::DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = swapChain->GetRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;

		pipeline = std::make_unique<Pipeline>(engineDevice,
			"shaders/simple_shader.vert.spv",
			"shaders/simple_shader.frag.spv",
			pipelineConfig);
	}
		
	void Bootstrap::CreateSwapChain()
	{
		auto extent = appWindow.GetExtent();
		while(extent.width == 0 || extent.height == 0)
		{
			extent = appWindow.GetExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(engineDevice.GetDevice());
		swapChain = nullptr;
		swapChain = std::make_unique<SwapChain>(engineDevice, extent);

		if(swapChain == nullptr)
		{
			swapChain = std::make_unique<SwapChain>(engineDevice, extent);
		}
		else
		{
			swapChain = std::make_unique<SwapChain>(engineDevice, extent, std::move(swapChain));

			if(swapChain->ImageCount() != commandBuffers.size())
			{
				FreeCommandBuffers();
				CreateCommandBuffers();
			}
		}
		CreatePipeline();
	}

	void Bootstrap::CreateCommandBuffers()
	{
		commandBuffers.resize(swapChain->ImageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = engineDevice.GetCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(engineDevice.GetDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffers!");
		}
	}

	void Bootstrap::FreeCommandBuffers()
	{
		if(!commandBuffers.empty())
		{
			vkFreeCommandBuffers(engineDevice.GetDevice(), engineDevice.GetCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
			commandBuffers.clear();
		}
	}

	void Bootstrap::DrawFrame()
	{
		uint32_t imageIndex;
		VkResult result = swapChain->AcquireNextImage(&imageIndex);

		if(result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			CreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire swap chain image!");
		}

		RecordCommandBuffer(imageIndex);
		result = swapChain->SubmitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

		if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || appWindow.WasWindowResized())
		{
			appWindow.ResetResizedFlag();
			CreateSwapChain();
		}
	}

	void Bootstrap::RecordCommandBuffer(const uint32_t imageIndex)
	{
		VkCommandBuffer_T* commandBuffer = commandBuffers[imageIndex];

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = swapChain->GetRenderPass();
		renderPassInfo.framebuffer = swapChain->GetFrameBuffer(imageIndex);
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = swapChain->GetSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {{0.01f, 0.01f, 0.01f, 1.0f}};
		clearValues[1].depthStencil = {1.0f, 0};

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		const VkViewport viewport{0.0f, 0.0f, static_cast<float>(swapChain->Width()), static_cast<float>(swapChain->Height()), 0.0f, 1.0f};
		const VkRect2D scissor{{0, 0}, swapChain->GetSwapChainExtent()};
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		RenderGameObjects(commandBuffer);

		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to record command buffer!");
		}
	}

	void Bootstrap::RenderGameObjects(const VkCommandBuffer commandBuffer)
	{
		pipeline->Bind(commandBuffer);

		for (GameObject& obj : gameObjects) 
		{
			obj.transform.rotation = glm::mod(obj.transform.rotation + 0.01f, glm::two_pi<float>());

			ConstantData push{};
			push.offset = obj.transform.translation;
			push.color = obj.color;
			push.transform = obj.transform.Mat2();
			vkCmdPushConstants(commandBuffer,pipelineLayout,VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,0,sizeof(ConstantData), &push);

			obj.model->Bind(commandBuffer);
			obj.model->Draw(commandBuffer);
		}
}
}
