#pragma once

#include "ApplicationWindow.h"
#include "EngineDevice.h"
#include "Model.h"
#include "Pipeline.h"
#include "SwapChain.h"

// std
#include <memory>
#include <vector>


namespace Goss
{
	class GameObject;

	class Bootstrap
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		Bootstrap();
		~Bootstrap();

		Bootstrap(const Bootstrap&) = delete;
		Bootstrap& operator=(const Bootstrap&) = delete;

		void Run();

	private:
		void LoadGameObjects();
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateSwapChain();
		void CreateCommandBuffers();
		void FreeCommandBuffers();
		void DrawFrame();
		void RecordCommandBuffer(uint32_t imageIndex);
		void RenderGameObjects(VkCommandBuffer commandBuffer);

		static void Sierpinski(std::vector<Model::Vertex>& vertices, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top);

		ApplicationWindow appWindow{WIDTH, HEIGHT, "Goss"};
		EngineDevice engineDevice{appWindow};
		std::unique_ptr<SwapChain> swapChain; 

		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout{};
		std::vector<VkCommandBuffer> commandBuffers;
		std::vector<GameObject> gameObjects;
	};
}
