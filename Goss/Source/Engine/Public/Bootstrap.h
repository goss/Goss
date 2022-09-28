#pragma once

#include "ApplicationWindow.h"
#include "Device.h"
#include "Model.h"
#include "Renderer.h"

// std
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

		std::unique_ptr<Model> CreateCubeModel(glm::vec3 offset);

		ApplicationWindow window{WIDTH, HEIGHT, "Goss"};
		Device device{window};
		Renderer renderer{window, device};

		std::vector<GameObject> gameObjects;
	};
}
