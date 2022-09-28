#include "Bootstrap.h"
#include "GameObject.h"
#include "RenderSystem.h"

// glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace Goss
{
	Bootstrap::Bootstrap()
	{
		LoadGameObjects();
	}

	Bootstrap::~Bootstrap() = default;

	void Bootstrap::Run()
	{
		const RenderSystem renderSystem{device, renderer.GetSwapChainRenderPass()};
		while (!window.ShouldClose())
		{
			glfwPollEvents();
			if (const VkCommandBuffer commandBuffer = renderer.BeginFrame()) 
			{
				renderer.BeginSwapChainRenderPass(commandBuffer);
				renderSystem.RenderGameObjects(commandBuffer, gameObjects);
				renderer.EndSwapChainRenderPass(commandBuffer);
				renderer.EndFrame();
			}
		}

		vkDeviceWaitIdle(device.GetDevice());
	}

	void Bootstrap::LoadGameObjects()
	{
		std::vector<Model::Vertex> vertices
		{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		};
		//Sierpinski(vertices, 3, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});

		std::vector<glm::vec3> colors
		{
			{1.f, .7f, .73f},
			{1.f, .87f, .73f},
			{1.f, 1.f, .73f},
			{.73f, 1.f, .8f},
			{.73, .88f, 1.f} 
		};

		for (auto& color : colors) 
		{
			color = pow(color, glm::vec3{2.2f});
		}

		for (int i = 0; i < 4; i++) 
		{
			GameObject triangle = GameObject::CreateGameObject();
			triangle.model = std::make_unique<Model>(device, vertices);
			triangle.transform.scale = glm::vec2(.5f) + i * 0.025f;
			triangle.transform.rotation = i * glm::pi<float>() * .025f;
			triangle.color = colors[i % colors.size()];
			gameObjects.push_back(std::move(triangle));
		}
	}

	void Bootstrap::Sierpinski(std::vector<Model::Vertex>& vertices, const int depth, const glm::vec2 left, const glm::vec2 right, const glm::vec2 top)
	{
		if (depth <= 0)
		{
			vertices.push_back({top});
			vertices.push_back({right});
			vertices.push_back({left});
		}
		else
		{
			const auto leftTop = 0.5f * (left + top);
			const auto rightTop = 0.5f * (right + top);
			const auto leftRight = 0.5f * (left + right);
			Sierpinski(vertices, depth - 1, left, leftRight, leftTop);
			Sierpinski(vertices, depth - 1, leftRight, right, rightTop);
			Sierpinski(vertices, depth - 1, leftTop, rightTop, top);
		}
	}
}
