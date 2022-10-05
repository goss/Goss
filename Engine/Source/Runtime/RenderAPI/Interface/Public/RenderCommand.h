#pragma once

#include "RendererAPI.h"
#include "VertexArray.h"

namespace Goss {

	class RenderCommand
	{
	public:
		static void Init()
		{
			rendererAPI->Init();
		}

		static void SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
		{
			rendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const glm::vec4& color)
		{
			rendererAPI->SetClearColor(color);
		}

		static void Clear()
		{
			rendererAPI->Clear();
		}

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t indexCount = 0)
		{
			rendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		static void DrawLines(const Ref<VertexArray>& vertexArray, const uint32_t vertexCount)
		{
			rendererAPI->DrawLines(vertexArray, vertexCount);
		}

		static void SetLineWidth(const float width)
		{
			rendererAPI->SetLineWidth(width);
		}
	private:
		static Scope<RendererAPI> rendererAPI;
	};

}
