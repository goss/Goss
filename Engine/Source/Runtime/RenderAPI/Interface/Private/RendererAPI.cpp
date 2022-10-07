#include "gepch.h"
#include "RendererAPI.h"

#include "Core.h"
#include "OpenGLRendererAPI.h"
#include "VulkanRendererAPI.h"

namespace Goss
{
	RendererAPI::API RendererAPI::api = API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (api)
		{
			case API::DirectX:
				GE_CORE_ASSERT(false, "DirectX not supported!")
				break;
			case API::OpenGL:
				return CreateScope<OpenGLRendererAPI>();
			case API::Vulkan:
				return CreateScope<VulkanRendererAPI>();
			default:
				GE_CORE_ASSERT(false, "Unknown RendererAPI!")
				break;
		}
		return nullptr;
	}
}
