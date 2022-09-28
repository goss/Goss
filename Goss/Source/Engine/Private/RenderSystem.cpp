#include "RenderSystem.h"
#include "Pipeline.h"

// glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <stdexcept>

namespace Goss
{
	/**
	* Limited Constant data size is 128 bytes
	* Used for instancing
	*/
	struct ConstantData
	{
		glm::mat2 transform{1.f};
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

	RenderSystem::RenderSystem(Device& device, VkRenderPass renderPass) : lveDevice{device}
	{
		CreatePipelineLayout();
		CreatePipeline(renderPass);
	}

	RenderSystem::~RenderSystem()
	{
		vkDestroyPipelineLayout(lveDevice.GetDevice(), pipelineLayout, nullptr);
	}

	void RenderSystem::CreatePipelineLayout()
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

		if (vkCreatePipelineLayout(lveDevice.GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	void RenderSystem::CreatePipeline(const VkRenderPass renderPass)
	{
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		Pipeline::DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		lvePipeline = std::make_unique<Pipeline>(
			lveDevice,
			"shaders/simple_shader.vert.spv",
			"shaders/simple_shader.frag.spv",
			pipelineConfig);
	}

	void RenderSystem::RenderGameObjects(const VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects) const
	{
		lvePipeline->Bind(commandBuffer);

		for (GameObject& gameObject : gameObjects)
		{
			gameObject.transform.rotation = glm::mod(gameObject.transform.rotation + 0.01f, glm::two_pi<float>());

			ConstantData push{};
			push.offset = gameObject.transform.translation;
			push.color = gameObject.color;
			push.transform = gameObject.transform.Mat2();

			vkCmdPushConstants(
				commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(ConstantData),
				&push);

			gameObject.model->Bind(commandBuffer);
			gameObject.model->Draw(commandBuffer);
		}
	}
}
