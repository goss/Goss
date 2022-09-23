#pragma once

#include "ApplicationWindow.h"
#include "EngineDevice.h"
#include "Pipeline.h"
#include "SwapChain.h"

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
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCommandBuffers();
		void DrawFrame();

		ApplicationWindow appWindow{WIDTH, HEIGHT, "Goss"};
		EngineDevice engineDevice{appWindow};
		SwapChain swapChain{engineDevice, appWindow.GetExtent()};

		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout{};
		std::vector<VkCommandBuffer> commandBuffers;
	};
}
