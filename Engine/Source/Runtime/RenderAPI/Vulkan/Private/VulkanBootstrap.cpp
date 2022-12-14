#include "gepch.h"

#include "VulkanBootstrap.h"

#include "Camera.h"
#include "GameObject.h"
#include "OrthographicCamera.h"
#include "VulkanRenderSystem.h"

namespace Goss
{
	VulkanBootstrap::VulkanBootstrap(Window& window) : window(window)
	{
		LoadGameObjects();
	}

	VulkanBootstrap::~VulkanBootstrap() = default;

	void VulkanBootstrap::Run(const Timestep timestep)
	{
		const VulkanRenderSystem renderSystem{device, renderer.GetSwapChainRenderPass()};
		const float aspect = renderer.GetAspectRatio();

		OrthographicCamera camera(-1, 1, 1, -1);
		//camera.SetViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));
		//camera.SetOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
		//camera.SetPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 100.f);

		renderSystem.Tick(timestep.GetSeconds() , gameObjects);

		if (const VkCommandBuffer commandBuffer = renderer.BeginFrame()) 
		{
			renderer.BeginSwapChainRenderPass(commandBuffer);
			renderSystem.Render(commandBuffer, gameObjects, camera);
			renderer.EndSwapChainRenderPass(commandBuffer);
			renderer.EndFrame();
		}

		vkDeviceWaitIdle(device.GetDevice());
	}

	//TODO Move to sandbox layer
	void VulkanBootstrap::LoadGameObjects()
	{
		const std::shared_ptr model = CreateCubeModel({0.0f, 0.0f, 0.0f});
		GameObject cube = GameObject::CreateGameObject();
		cube.model = model;
		cube.transform.position = {0.0f, 0.0f, 2.5f};
		cube.transform.scale = {0.5f, 0.5f, 0.5f};
		cube.color = glm::vec3{1.0f, 0.0f, 0.0f};

		gameObjects.push_back(std::move(cube));
	}

	std::unique_ptr<Model> VulkanBootstrap::CreateCubeModel(const glm::vec3 offset)
	{
		std::vector<Model::Vertex> vertices
		{
      // left face (white)
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

      // right face (yellow)
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

      // top face (orange, remember y axis points down)
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

      // bottom face (red)
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

      // nose face (blue)
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

      // tail face (green)
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
		};
		for (auto& v : vertices) 
		{
			v.position += offset;
		}
	  return std::make_unique<Model>(device, vertices);
	}
}

