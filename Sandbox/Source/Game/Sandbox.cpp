#include <Engine.h>

#include "SandboxLayer.h"

class Sandbox final : public Goss::Application
{
public:
	explicit Sandbox(const Goss::ApplicationSpecification& specification) : Application(specification)
	{
		PushLayer(new SandboxLayer());
	}
	~Sandbox() override = default;
};

Goss::Application* CreateApplication(const Goss::ApplicationCommandLineArgs args)
{
	GE_CORE_INFO("Creating Sandbox Application");

	Goss::ApplicationSpecification spec;
	spec.name = "Sandbox";
	spec.workingDirectory = "../Sandbox";
	spec.commandLineArgs = args;

	return new Sandbox(spec);
}
