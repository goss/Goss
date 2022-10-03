#include <Engine.h>

namespace Goss
{
	class Sandbox final : public Application
	{
	public:
		Sandbox() = default;
		~Sandbox() override = default;
	};

	Application* CreateApplication()
	{
		return new Sandbox();
	}
}



