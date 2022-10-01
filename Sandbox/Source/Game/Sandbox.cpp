#include <Engine.h>

class Sandbox : public Goss::Application
{
};

Goss::Application* Goss::CreateApplication()
{
	GE_WARN("Creating Sandbox");
	return new Sandbox();
}