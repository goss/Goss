#include "SandboxLayer.h"

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

SandboxLayer::SandboxLayer() 
	: Layer("SandboxLayer"), cameraController(1280.0f / 720.0f)
{
	vertexArray = Goss::VertexArray::Create();

	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};

	const Goss::Ref<Goss::VertexBuffer> vertexBuffer = Goss::VertexBuffer::Create(vertices, sizeof(vertices));
	const Goss::BufferLayout layout = 
	{
		{ Goss::ShaderDataType::Float3, "a_Position" },
		{ Goss::ShaderDataType::Float4, "a_Color" }
	};
	vertexBuffer->SetLayout(layout);
	vertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t indices[3] = { 0, 1, 2 };
	const Goss::Ref<Goss::IndexBuffer> indexBuffer = Goss::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	vertexArray->SetIndexBuffer(indexBuffer);

	squareVertexArray = Goss::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	const Goss::Ref<Goss::VertexBuffer> squareVb = Goss::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVb->SetLayout({
		{ Goss::ShaderDataType::Float3, "a_Position" },
		{ Goss::ShaderDataType::Float2, "a_TexCoord" }
		});
	squareVertexArray->AddVertexBuffer(squareVb);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	const Goss::Ref<Goss::IndexBuffer> squareIb = Goss::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	squareVertexArray->SetIndexBuffer(squareIb);

	const std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";


	const std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";


	const std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

	const std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			
			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

	shader = Goss::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);
	flatColorShader = Goss::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

	const auto textureShader = shaderLibrary.Load("Assets/Shaders/Texture.glsl");
	textureShader->Bind();
	textureShader->SetInt("u_Texture", 0);

	texture = Goss::Texture2D::Create("Assets/Textures/Checkerboard.png");
}

void SandboxLayer::OnAttach()
{
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::Tick(const Goss::Timestep timestep) 
{
	// Update
	cameraController.Tick(timestep);

	// Render
	Goss::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Goss::RenderCommand::Clear();

	Goss::Renderer::BeginScene(cameraController.GetCamera());

	//const glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	//flatColorShader->Bind();
	//flatColorShader->SetFloat3("u_Color", squareColor);

	//for (int y = 0; y < 20; y++)
	//{
	//	for (int x = 0; x < 20; x++)
	//	{
	//		glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
	//		glm::mat4 transform = translate(glm::mat4(1.0f), pos) * scale;
	//		Goss::Renderer::Submit(flatColorShader, squareVertexArray, transform);
	//	}
	//}

	const Goss::Ref<Goss::Shader> textureShader = shaderLibrary.Get("Texture");

	texture->Bind();
	Goss::Renderer::Submit(textureShader, squareVertexArray, scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	// Triangle
	Goss::Renderer::Submit(shader, vertexArray);

	Goss::Renderer::EndScene();
}

void SandboxLayer::OnEvent(Goss::Event& e) 
{
	cameraController.OnEvent(e);
}
