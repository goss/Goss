#include "gepch.h"

#include "Core.h"
#include "Renderer.h"
#include "RendererAPI.h"

namespace Goss
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::Vulkan:
			case RendererAPI::API::OpenGL:
				GE_CORE_ASSERT(false, "OpenGL not supported!")
				break;
			case RendererAPI::API::DirectX:
				GE_CORE_ASSERT(false, "DirectX not supported!")
				break;
			default:
				GE_CORE_ASSERT(false, "Unknown RendererAPI!")
				break;
		}

		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::Vulkan:
			case RendererAPI::API::OpenGL:
				GE_CORE_ASSERT(false, "OpenGL not supported!")
				break;
			case RendererAPI::API::DirectX:
				GE_CORE_ASSERT(false, "DirectX not supported!")
				break;
			default:
				GE_CORE_ASSERT(false, "Unknown RendererAPI!")
				break;
		}

		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::Vulkan:
			case RendererAPI::API::OpenGL:
				GE_CORE_ASSERT(false, "OpenGL not supported!")
				break;
			case RendererAPI::API::DirectX:
				GE_CORE_ASSERT(false, "DirectX not supported!")
				break;
			default:
				GE_CORE_ASSERT(false, "Unknown RendererAPI!")
				break;
		}

		return nullptr;
	}

}
