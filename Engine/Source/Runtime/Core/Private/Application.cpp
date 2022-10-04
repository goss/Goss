#include "gepch.h"

#include "Application.h"
#include "VulkanBootstrap.h"
#include "Log.h"

namespace Goss
{
	Application::Application() = default;
	Application::~Application() = default;

	void Application::Run()
	{
		VulkanBootstrap bootstrap{};
		bootstrap.Run();
	}
}

Goss::Application* CreateApplication()
{
	return new Goss::Application();
}

int main(int argc, char** argv)
{
	Goss::Log::Init();
	GE_CORE_INFO("Creating Application");

	const auto application = Goss::CreateApplication();
	application->Run();
	delete application;
	return 0;
}

