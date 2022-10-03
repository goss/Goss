#include "gepch.h"

#include "RenderSystem.h"
#include "Pipeline.h"

// glm
#include <glm.hpp>
#include <gtc/constants.hpp>

namespace Goss
{
	/**
	* Limited Constant data size is 128 bytes
	* Can be used for instancing
	*/
	struct PushConstantData
	{
		glm::mat4 transform{1.f};
		alignas(16) glm::vec3 color{};
	};

	RenderSystem::RenderSystem(Device& device, const VkRenderPass renderPass) : lveDevice{device}
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
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PushConstantData);

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
			"../engine/shaders/simple_shader.vert.spv",
			"../engine/shaders/simple_shader.frag.spv",
			pipelineConfig);
	}

	void RenderSystem::Tick(const float deltaTime, std::vector<GameObject>& gameObjects) const
	{
		for (GameObject& gameObject : gameObjects)
		{
			gameObject.transform.rotation.x = glm::mod(gameObject.transform.rotation.x + (0.1f * deltaTime), glm::two_pi<float>());
			gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y + (0.1f * deltaTime), glm::two_pi<float>());
		}
	}

	void RenderSystem::Render(const VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const Camera& camera) const
	{
		lvePipeline->Bind(commandBuffer);

		const auto projectionView = camera.GetProjection() * camera.GetView();

		for (GameObject& gameObject : gameObjects)
		{
			PushConstantData push{};
			push.color = gameObject.color;
			push.transform = projectionView * gameObject.transform.Mat4();

			vkCmdPushConstants(
				commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(PushConstantData),
				&push);

			gameObject.model->Bind(commandBuffer);
			gameObject.model->Draw(commandBuffer);
		}
	}
}
