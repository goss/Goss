#include "gepch.h"
#include "Runtime/RenderAPI/OpenGL/Public/OpenGLRendererAPI.h"

#include <glad/glad.h>

#include "Core.h"

namespace Goss
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		const unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         GE_CORE_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       GE_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          GE_CORE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: GE_CORE_TRACE(message); return;
			default: ;
		}
		
		GE_CORE_ASSERT(false, "Unknown severity level!");
	}

	void OpenGLRendererAPI::Initialize()
	{
	#ifdef GE_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
	#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLRendererAPI::SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t indexCount)
	{
		vertexArray->Bind();
		const uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, const uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void OpenGLRendererAPI::SetLineWidth(const float width)
	{
		glLineWidth(width);
	}

}
