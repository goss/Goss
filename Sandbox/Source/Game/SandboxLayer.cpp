#include "SandboxLayer.h"

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

SandboxLayer::SandboxLayer() 
	: Layer("SandboxLayer"), cameraController(1280.0f / 720.0f)
{
	vertexArray = VertexArray::Create();

	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};

	const Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
	const BufferLayout layout = 
	{
		{ShaderDataType::Float3, "a_Position" },
		{ShaderDataType::Float4, "a_Color" }
	};
	vertexBuffer->SetLayout(layout);
	vertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t indices[3] = { 0, 1, 2 };
	const Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	vertexArray->SetIndexBuffer(indexBuffer);

	squareVertexArray = VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	const Ref<VertexBuffer> squareVb = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVb->SetLayout({
		{ShaderDataType::Float3, "a_Position" },
		{ShaderDataType::Float2, "a_TexCoord" }
		});
	squareVertexArray->AddVertexBuffer(squareVb);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	const Ref<IndexBuffer> squareIb = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	squareVertexArray->SetIndexBuffer(squareIb);

	flatColorShader = shaderLibrary.Load("Assets/Shaders/FlatColor.glsl");
	textureShader = shaderLibrary.Load("Assets/Shaders/Texture.glsl");

	//flatColorShader->Bind();
	//flatColorShader->SetFloat3("u_Color", glm::vec4 { 0.2f, 0.3f, 0.8f, 1.0f });

	//textureShader->Bind();
	//textureShader->SetInt("u_Texture", 0);

	texture = Texture2D::Create("Assets/Textures/Checkerboard.png");
}

void SandboxLayer::OnAttach()
{
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::Tick(const Timestep timestep) 
{
	return;
	// Update
	cameraController.Tick(timestep);

	// Render
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();

	Renderer::BeginScene(cameraController.GetCamera());

	const glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = translate(glm::mat4(1.0f), pos) * scale;
			Renderer::Submit(flatColorShader, squareVertexArray, transform);
		}
	}

	texture->Bind();
	Renderer::Submit(textureShader, squareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	//Renderer::Submit(flatColorShader, vertexArray); // Triangle

	Renderer::EndScene();
}

void SandboxLayer::OnEvent(Event& e) 
{
	cameraController.OnEvent(e);
}
