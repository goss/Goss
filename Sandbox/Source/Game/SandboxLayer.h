#pragma once

#include "Engine.h"
#include "OrthographicCameraController.h"

using namespace Goss;

class SandboxLayer final : public Layer
{
public:
	SandboxLayer();
	~SandboxLayer() override = default;

	void OnAttach() override;
	void OnDetach() override;

	void Tick(Timestep timestep) override;
	void OnEvent(Event& e) override;
private:
	ShaderLibrary shaderLibrary;

	Ref<Shader> flatColorShader;
	Ref<Shader> textureShader;

	Ref<VertexArray> vertexArray;
	Ref<VertexArray> squareVertexArray;
	Ref<Texture2D> texture;

	OrthographicCameraController cameraController;
};

