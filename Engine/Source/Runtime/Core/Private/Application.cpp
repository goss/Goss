#include "gepch.h"

#include "Application.h"

#include "ApplicationEvent.h"
#include "PlatformUtils.h"
#include "Renderer.h"

namespace Goss
{
	Application* Application::instance = nullptr;

	Application::Application(ApplicationSpecification spec) : specification(std::move(spec))
	{
		instance = this;

		window = Window::Create(WindowProperties{specification.name});
		window->SetEventCallback(GE_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Initialize();
	}

	Application::~Application()
	{
		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		layerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& event)
	{
		//GE_CORE_INFO(event.ToString());

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(GE_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(GE_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = layerStack.rbegin(); it != layerStack.rend(); ++it)
		{
			if (event.handled) break;
			(*it)->OnEvent(event);
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		isRunning = false;
		return true;
	}

	bool Application::OnWindowResize(const WindowResizeEvent& event)
	{
		const unsigned int width = event.GetWidth();
		const unsigned int height = event.GetHeight();

		if (width == 0 || height == 0)
		{
			isMinimized = true;
			return false;
		}

		isMinimized = false;

		Renderer::OnWindowResize(width, height);

		return false;
	}

	void Application::Run()
	{
		while (isRunning)
		{
			const float time = Time::GetTime();
			const Timestep timeStep = time - lastFrameTime;
			lastFrameTime = time;

			if (!isMinimized)
			{
				for (Layer* layer : layerStack)
				{
					layer->Tick(timeStep);
				}
			}
			window->Tick();
		}
	}

	void Application::Close()
	{
		isRunning = false;
	}
}

int main(const int argc, char** argv)
{
	Goss::Log::Init();
	GE_CORE_INFO("Creating Application");

	const auto application = CreateApplication({ argc, argv });
	application->Run();
	delete application;
	return 0;
}

