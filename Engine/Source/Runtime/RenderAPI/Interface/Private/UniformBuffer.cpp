#include "gepch.h"
#include "UniformBuffer.h"

#include "Core.h"
#include "Renderer.h"
#include "RendererAPI.h"

namespace Goss
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
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
