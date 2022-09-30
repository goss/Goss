#include <Engine.h>

class Sandbox : public Goss::Application
{
public:
	~Sandbox() override = default;
};

Goss::Application* Goss::CreateApplication()
{
	return new Sandbox();
}