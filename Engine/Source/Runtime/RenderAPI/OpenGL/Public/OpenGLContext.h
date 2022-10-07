#pragma once

#include "GraphicsContext.h"

struct GLFWwindow;

namespace Goss
{
	class OpenGLContext final : public GraphicsContext
	{
	public:
		explicit OpenGLContext(GLFWwindow* windowHandle);

		void Initialize() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* windowHandle;
	};

}