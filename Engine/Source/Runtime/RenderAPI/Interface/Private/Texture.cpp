#include "gepch.h"
#include "Texture.h"

#include "Core.h"
#include "OpenGLTexture.h"
#include "Renderer.h"
#include "RendererAPI.h"

namespace Goss
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::DirectX:
				GE_CORE_ASSERT(false, "DirectX not supported!")
				break;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(width, height);
			case RendererAPI::API::Vulkan:
				GE_CORE_ASSERT(false, "Vulkan not supported!")
				break;
			default:
				GE_CORE_ASSERT(false, "Unknown RendererAPI!")
				break;
		}
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::DirectX:
				GE_CORE_ASSERT(false, "DirectX not supported!")
				break;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(path);
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
