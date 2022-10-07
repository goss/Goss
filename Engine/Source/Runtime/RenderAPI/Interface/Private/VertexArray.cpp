#include "gepch.h"
#include "VertexArray.h"

#include "Core.h"
#include "OpenGLVertexArray.h"
#include "Renderer.h"
#include "RendererAPI.h"

namespace Goss
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::DirectX:
				GE_CORE_ASSERT(false, "DirectX not supported!")
				break;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexArray>();
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
