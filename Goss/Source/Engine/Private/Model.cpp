// std
#include "Model.h"

#include <cassert>
#include <cstring>

namespace Goss
{
	Model::Model(EngineDevice& device, const std::vector<Vertex>& vertices) : device{device}
	{
		CreateVertexBuffers(vertices);
	}

	Model::~Model()
	{
		vkDestroyBuffer(device.Device(), vertexBuffer, nullptr);
		vkFreeMemory(device.Device(), vertexBufferMemory, nullptr);
	}

	void Model::CreateVertexBuffers(const std::vector<Vertex>& vertices)
	{
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3");
		const VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
		device.CreateBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, //CPU
			vertexBuffer,
			vertexBufferMemory);

		void* data;
		vkMapMemory(device.Device(), vertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), bufferSize);
		vkUnmapMemory(device.Device(), vertexBufferMemory);
	}

	void Model::Bind(const VkCommandBuffer commandBuffer) const
	{
		const VkBuffer buffers[] = {vertexBuffer};
		constexpr VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	void Model::Draw(const VkCommandBuffer commandBuffer) const
	{
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

	std::vector<VkVertexInputBindingDescription> Model::Vertex::GetBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> Model::Vertex::GetAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = 0;
		return attributeDescriptions;
	}
}
