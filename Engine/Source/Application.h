#pragma once
#include "Core.h"

//#include "Bootstrap.h"

namespace Goss
{
	class GOSS_API Application
	{

	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		//Bootstrap bootstrap{};
	};

	Application* CreateApplication();
}
