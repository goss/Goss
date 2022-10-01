#pragma once
#include "Core.h"

namespace Goss
{
	class GOSS_API Application
	{
	public:
		Application();
		void Run();
	};

	Application* CreateApplication();
}
