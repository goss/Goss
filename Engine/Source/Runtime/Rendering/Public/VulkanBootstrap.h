#pragma once

#include "VulkanDevice.h"
#include "GameObject.h"
#include "Model.h"
#include "VulkanRenderer.h"
#include "VulkanWindow.h"

namespace Goss
{
	class VulkanBootstrap
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		VulkanBootstrap();
		~VulkanBootstrap();

		VulkanBootstrap(const VulkanBootstrap&) = delete;
		VulkanBootstrap& operator=(const VulkanBootstrap&) = delete;

		void Run();

	private:
		void LoadGameObjects();

		std::unique_ptr<Model> CreateCubeModel(glm::vec3 offset);

		VulkanWindow window{WIDTH, HEIGHT, "Goss"};
		VulkanDevice device{window};
		VulkanRenderer renderer{window, device};

		std::vector<GameObject> gameObjects;

		//game timer
		std::chrono::time_point<std::chrono::steady_clock> currentTime;

		const float deltaTime = 0.01f;
		float frameTime = 0.0f;
		float accumulator = 0.0f;

		float elapsedTime = 0.0;
	};
}
