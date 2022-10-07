#pragma once

#include "GameObject.h"
#include "Model.h"
#include "Timestep.h"
#include "VulkanDevice.h"
#include "VulkanRenderer.h"

namespace Goss
{
	class VulkanBootstrap
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		VulkanBootstrap(Window& window);
		~VulkanBootstrap();

		VulkanBootstrap(const VulkanBootstrap&) = delete;
		VulkanBootstrap& operator=(const VulkanBootstrap&) = delete;

		void Run(Timestep timestep);

	private:
		void LoadGameObjects();
		std::unique_ptr<Model> CreateCubeModel(glm::vec3 offset);
		std::vector<GameObject> gameObjects;

		Window& window;
		VulkanDevice device{window};
		VulkanRenderer renderer{window, device};
	};
}
