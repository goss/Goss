#pragma once

#include "Event.h"
#include "Timestep.h"

namespace Goss
{
	class Layer
	{
	public:
		explicit Layer(std::string name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate(Timestep timeStep) {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return debugName; }
	protected:
		std::string debugName;
	};
}
