#pragma once

#include "Camera.h"
#include "GameObject.h"
#include "Pipeline.h"

namespace Goss
{
	class RenderSystem
	{
		public:
		RenderSystem(Device &device, VkRenderPass renderPass);
		~RenderSystem();

		RenderSystem(const RenderSystem &) = delete;
		RenderSystem &operator=(const RenderSystem &) = delete;

		void Tick(float deltaTime, std::vector<GameObject>& gameObjects) const;
		void Render(VkCommandBuffer commandBuffer, std::vector<GameObject> &gameObjects, const Camera& camera) const;

		private:
		void CreatePipelineLayout();
		void CreatePipeline(VkRenderPass renderPass);

		Device &lveDevice;

		std::unique_ptr<Pipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
	};
} 