#include "Application.h"

#include "Bootstrap.h"

namespace Goss
{
	Application::Application() = default;

	void Application::Run()
	{
		Bootstrap bootstrap{};
		bootstrap.Run();
	}
}

