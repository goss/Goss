#include "gepch.h"
#include "RendererAPI.h"

#include "Core.h"

namespace Goss
{
	RendererAPI::API RendererAPI::api = API::Vulkan;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (api)
		{
			case API::Vulkan:
			case API::OpenGL:
				GE_CORE_ASSERT(false, "OpenGL not supported!")
				break;
			case API::DirectX:
				GE_CORE_ASSERT(false, "DirectX not supported!")
				break;
			default:
				GE_CORE_ASSERT(false, "Unknown RendererAPI!")
				break;
		}

		return nullptr;
	}

}
