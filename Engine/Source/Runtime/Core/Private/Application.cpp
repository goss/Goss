#include "gepch.h"

#include "Application.h"

namespace Goss
{
	Application* Application::instance = nullptr;

	Application::Application()
	{
		instance = this;

		window = Window::Create(WindowProperties("Goss"));
		window->SetEventCallback(GE_BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application() = default;

	void Application::OnEvent(const Event& e)
	{
		GE_CORE_INFO(e.ToString());
	}

	void Application::Run()
	{
		while (true)
		{
			window->OnUpdate();
		}
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

