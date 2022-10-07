#include "gepch.h"
#include "GraphicsContext.h"

#include "Core.h"
#include "OpenGLContext.h"
#include "Renderer.h"
#include "RendererAPI.h"

namespace Goss
{
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::DirectX:
				GE_CORE_ASSERT(false, "DirectX not supported!")
				break;
			case RendererAPI::API::OpenGL:
				return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
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
