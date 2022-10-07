#pragma once

#include "Engine.h"
#include "OrthographicCameraController.h"

class SandboxLayer final : public Goss::Layer
{
public:
	SandboxLayer();
	~SandboxLayer() override = default;

	void OnAttach() override;
	void OnDetach() override;

	void Tick(Goss::Timestep timestep) override;
	void OnEvent(Goss::Event& e) override;
private:
	Goss::ShaderLibrary shaderLibrary;
	Goss::Ref<Goss::Shader> shader;
	Goss::Ref<Goss::VertexArray> vertexArray;

	Goss::Ref<Goss::Shader> flatColorShader;
	Goss::Ref<Goss::VertexArray> squareVertexArray;

	Goss::Ref<Goss::Texture2D> texture;

	Goss::OrthographicCameraController cameraController;
	glm::vec3 squareColor = { 0.2f, 0.3f, 0.8f };
};

