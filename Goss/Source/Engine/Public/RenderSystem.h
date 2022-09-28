#pragma once
#include "GameObject.h"
#include "Pipeline.h"

namespace Goss
{
	class RenderSystem
	{
		public:
		RenderSystem(EngineDevice &device, VkRenderPass renderPass);
		~RenderSystem();

		RenderSystem(const RenderSystem &) = delete;
		RenderSystem &operator=(const RenderSystem &) = delete;

		void RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject> &gameObjects) const;

		private:
		void CreatePipelineLayout();
		void CreatePipeline(VkRenderPass renderPass);

		EngineDevice &lveDevice;

		std::unique_ptr<Pipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
	};
} 