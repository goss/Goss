#pragma once
#include "Core.h"

namespace Goss
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;
		virtual void SwapBuffers() = 0;

		static Scope<GraphicsContext> Create(void* window);
	protected:
			virtual void Initialize() = 0;
	};

}
