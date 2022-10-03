#include "gepch.h"

#include "Application.h"
#include "Bootstrap.h"

namespace Goss
{
	Application::Application() = default;
	Application::~Application() = default;

	void Application::Run()
	{
		Bootstrap bootstrap{};
		bootstrap.Run();
	}
}

Goss::Application* CreateApplication()
{
	return new Goss::Application();
}

int main(int argc, char** argv)
{
	const auto application = Goss::CreateApplication();
	application->Run();
	delete application;
	return 0;
}

