#pragma once
#include "ApplicationEvent.h"
#include "Core.h"
#include "Event.h"
#include "Window.h"

int main(int argc, char** argv);

namespace Goss
{
	class Application
	{
	public:
		Application();
		virtual ~Application();


		void Run();

		static Application& Get() { return *instance; }
		Window& GetWindow() const { return *window; }
		
	private:
		void OnEvent(Event& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);

		Scope<Window> window;

		bool isRunning = true;
		bool isMinimized = false;

		static Application* instance;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication();
}

extern Goss::Application* CreateApplication();
