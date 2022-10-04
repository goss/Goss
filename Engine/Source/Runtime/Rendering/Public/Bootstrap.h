#pragma once

#include "Device.h"
#include "GameObject.h"
#include "Model.h"
#include "Renderer.h"
#include "Window.h"

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
		void LoadGameObjects();

		std::unique_ptr<Model> CreateCubeModel(glm::vec3 offset);

		Window window{WIDTH, HEIGHT, "Goss"};
		Device device{window};
		Renderer renderer{window, device};

		std::vector<GameObject> gameObjects;

		//game timer
		std::chrono::time_point<std::chrono::steady_clock> currentTime;

		const float deltaTime = 0.01f;
		float frameTime = 0.0f;
		float accumulator = 0.0f;

		float elapsedTime = 0.0;
	};
}
