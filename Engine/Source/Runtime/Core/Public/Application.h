#pragma once
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
		static Application* instance;
		Scope<Window> window;
		friend int ::main(int argc, char** argv);

		void OnEvent(const Event& e);
	};

	Application* CreateApplication();
}

extern Goss::Application* CreateApplication();
