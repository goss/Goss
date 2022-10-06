#pragma once
#include "VertexArray.h"

#include <glm.hpp>

namespace Goss
{
	class RendererAPI
	{
	public:
		enum class API
		{
			Vulkan = 1,
			OpenGL = 2,
			DirectX = 3
		};

		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;
		
		virtual void SetLineWidth(float width) = 0;

		static API GetAPI() { return api; }
		static Scope<RendererAPI> Create();
	private:
		static API api;
	};
}
