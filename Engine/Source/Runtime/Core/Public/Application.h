#pragma once
#include "ApplicationEvent.h"
#include "Core.h"
#include "Event.h"
#include "LayerStack.h"
#include "Window.h"

int main(int argc, char** argv);

namespace Goss
{
	struct ApplicationCommandLineArgs
	{
		int count = 0;
		char** args = nullptr;

		const char* operator[](const int index) const
		{
			//GE_CORE_ASSERT(index < count);
			return args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string name = "Goss Application";
		std::string workingDirectory;
		ApplicationCommandLineArgs commandLineArgs;
	};

	class Application
	{
	public:
		explicit Application(ApplicationSpecification specification);
		virtual ~Application();

		void Run();
		void Close();

		static Application& Get() { return *instance; }
		Window& GetWindow() const { return *window; }
		const ApplicationSpecification& GetSpecification() const { return specification; }

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		
	private:
		void OnEvent(Event& event);
		bool OnWindowResize(const WindowResizeEvent& event);
		bool OnWindowClose(WindowCloseEvent& event);

		Scope<Window> window;
		ApplicationSpecification specification;

		bool isRunning = true;
		bool isMinimized = false;

		LayerStack layerStack;
		float lastFrameTime = 0.0f;

		static Application* instance;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);
}

extern Goss::Application* CreateApplication(Goss::ApplicationCommandLineArgs args);
