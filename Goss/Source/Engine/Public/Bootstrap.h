#pragma once

#include "ApplicationWindow.h"
#include "EngineDevice.h"
#include "Pipeline.h"
#include "SwapChain.h"
#include "Model.h"

// std
#include <memory>
#include <vector>

namespace Goss
{
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
		void LoadModels();
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCommandBuffers();
		void FreeCommandBuffers();
		void DrawFrame();
		void CreateSwapChain();
		void RecordCommandBuffer(uint32_t imageIndex) const;

		static void Sierpinski(std::vector<Model::Vertex>& vertices, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top);

		ApplicationWindow appWindow{WIDTH, HEIGHT, "Goss"};
		EngineDevice engineDevice{appWindow};
		std::unique_ptr<SwapChain> swapChain; 

		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout{};
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<Model> model;
	};
}
