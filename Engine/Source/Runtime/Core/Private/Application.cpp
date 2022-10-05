#include "gepch.h"

#include "Application.h"

#include "ApplicationEvent.h"

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

	void Application::OnEvent(Event& e)
	{
		GE_CORE_INFO(e.ToString());

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(GE_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(GE_BIND_EVENT_FN(Application::OnWindowResize));
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		isRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			isMinimized = true;
			return false;
		}

		isMinimized = false;

		//TODO Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	void Application::Run()
	{
		while (isRunning)
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

