#include "gepch.h"

#include "VulkanRendererAPI.h"
#include "Core.h"

namespace Goss
{
	void VulkanRendererAPI::Initialize()
	{
		GE_CORE_INFO("Initializing VulkanRendererAPI");
	}

	void VulkanRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
	}

	void VulkanRendererAPI::SetClearColor(const glm::vec4& color)
	{
	}

	void VulkanRendererAPI::Clear()
	{
	}

	void VulkanRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
	}

	void VulkanRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
	}

	void VulkanRendererAPI::SetLineWidth(float width)
	{
	}
}
