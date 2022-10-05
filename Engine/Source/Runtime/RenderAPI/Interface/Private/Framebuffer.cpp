#include "gepch.h"
#include "Framebuffer.h"

#include "Core.h"
#include "Renderer.h"
#include "RendererAPI.h"

namespace Goss
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
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

