#pragma once

#include "Camera.h"
#include "GameObject.h"
#include "VulkanPipeline.h"

namespace Goss
{
	class VulkanRenderSystem
	{
	public:
		VulkanRenderSystem(VulkanDevice &device, VkRenderPass renderPass);
		~VulkanRenderSystem();

		VulkanRenderSystem(const VulkanRenderSystem &) = delete;
		VulkanRenderSystem &operator=(const VulkanRenderSystem &) = delete;

		void Tick(float deltaTime, std::vector<GameObject>& gameObjects) const;
		void Render(VkCommandBuffer commandBuffer, std::vector<GameObject> &gameObjects, const Camera& camera) const;

	private:
		void CreatePipelineLayout();
		void CreatePipeline(VkRenderPass renderPass);

		VulkanDevice& device;

		std::unique_ptr<VulkanPipeline> pipeline;
		VkPipelineLayout pipelineLayout;
	};
} 