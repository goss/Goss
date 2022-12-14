#include "gepch.h"
#include "Renderer.h"

#include "Core.h"
#include "OrthographicCamera.h"
#include "RenderCommand.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Goss
{
	Scope<Renderer::SceneData> Renderer::sceneData = CreateScope<SceneData>();

	void Renderer::Initialize()
	{
		RenderCommand::Initialize();
		//Renderer2D::Initialize();
	}

	void Renderer::Shutdown()
	{
		//Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(const uint32_t width, const uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const Camera& camera)
	{
		sceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetTransformAndViewMatrix(sceneData->viewProjectionMatrix, transform);

		RenderCommand::DrawIndexed(vertexArray);
	}
}
