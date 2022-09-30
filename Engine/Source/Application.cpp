#include "Application.h"

namespace Goss
{
	Application::Application() = default;
	Application::~Application() = default;

	void Application::Run()
	{
		bootstrap.Run();

	}
}

