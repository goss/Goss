#include "SandboxLayer.h"

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

SandboxLayer::SandboxLayer() 
	: Layer("SandboxLayer"), cameraController(1280.0f / 720.0f)
{
	float vertices[3 * 3] = 
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	const Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
	const BufferLayout layout = 
	{
		{ShaderDataType::Float3, "position" },
	};
	vertexBuffer->SetLayout(layout);

	vertexArray = VertexArray::Create();
	vertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t indices[3] = { 0, 1, 2 };
	const Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	vertexArray->SetIndexBuffer(indexBuffer);

	flatColorShader = shaderLibrary.Load("FlatColor");
	//textureShader = shaderLibrary.Load("Texture");

	flatColorShader->Bind();
	//flatColorShader->SetFloat4("color", glm::vec4 { 1.0f, 1.0f, 0.0f, 1.0f });
	flatColorShader->SetColor( glm::vec4 { 1.0f, 0.5f, 0.0f, 1.0f });

	//textureShader->Bind();
	//textureShader->SetInt("u_Texture", 0);

	//texture = Texture2D::Create("Assets/Textures/Checkerboard.png");
}

void SandboxLayer::OnAttach()
{
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::Tick(const Timestep timestep) 
{
	cameraController.Tick(timestep);

	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	RenderCommand::Clear();

	Renderer::BeginScene(cameraController.GetCamera());

	Renderer::Submit(flatColorShader, vertexArray); 

	Renderer::EndScene();
}

void SandboxLayer::OnEvent(Event& e) 
{
	cameraController.OnEvent(e);
}
