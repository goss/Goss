#pragma once
#include "EngineDevice.h"

// glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace Goss
{
	class Model
	{
	public:
		struct Vertex
		{
			glm::vec2 position;
			glm::vec3 color{1.0, 0.5, 0.0};

			static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
		};

		Model(EngineDevice& device, const std::vector<Vertex>& vertices);
		~Model();

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void Bind(VkCommandBuffer commandBuffer) const;
		void Draw(VkCommandBuffer commandBuffer) const;
		
	private:
		void CreateVertexBuffers(const std::vector<Vertex>& vertices);

		EngineDevice& device;
		VkBuffer vertexBuffer{};
		VkDeviceMemory vertexBufferMemory{};
		uint32_t vertexCount{};
	};
}
