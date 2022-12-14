#include "SandboxLayer.h"
#include "LuaTest.h"

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
	flatColorShader->SetColor(glm::vec4 { 1.0f, 0.5f, 0.0f, 1.0f });

	//textureShader->Bind();
	//textureShader->SetInt("u_Texture", 0);

	//texture = Texture2D::Create("Assets/Textures/Checkerboard.png");

	LuaTest::Run();
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

	const glm::mat4 transform = glm::mat4(1.0f);
	static float angle = 0;
	angle += 0.001f * timestep.GetMilliseconds();
	const auto mat = rotate(transform, angle, glm::vec3(0, 0, 1));

	glm::vec3 pos = glm::vec3(1, 1, 1);

	Renderer::BeginScene(cameraController.GetCamera());

	Renderer::Submit(flatColorShader, vertexArray, mat); 

	Renderer::EndScene();
}

void SandboxLayer::OnEvent(Event& e) 
{
	cameraController.OnEvent(e);
}
