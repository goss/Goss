#include "gepch.h"
#include "UniformBuffer.h"

#include "Core.h"
#include "OpenGLUniformBuffer.h"
#include "Renderer.h"
#include "RendererAPI.h"

namespace Goss
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::DirectX:
				GE_CORE_ASSERT(false, "DirectX not supported!")
				break;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLUniformBuffer>(size, binding);
			case RendererAPI::API::Vulkan:
				GE_CORE_ASSERT(false, "Vulkan not supported!")
				break;
			default:
				GE_CORE_ASSERT(false, "Unknown RendererAPI!")
				break;
		}
		return nullptr;
	}
}
