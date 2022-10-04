#pragma once
#include "Core.h"

int main(int argc, char** argv);

namespace Goss
{
	class GOSS_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	private:
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication();
}

extern Goss::Application* CreateApplication();
