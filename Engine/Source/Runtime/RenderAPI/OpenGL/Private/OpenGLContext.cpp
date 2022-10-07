#include "gepch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Goss
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: windowHandle(windowHandle)
	{
		GE_CORE_ASSERT(windowHandle, "Window handle is null!")
		Initialize();
	}

	void OpenGLContext::Initialize()
	{
		glfwMakeContextCurrent(windowHandle);
		const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		GE_CORE_ASSERT(status, "Failed to initialize Glad!")

		GE_CORE_INFO("OpenGL Info:");
		GE_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		GE_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		GE_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		if(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5))
		{
			GE_CORE_INFO("Requires at least OpenGL version 4.5!");
		}
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(windowHandle);
	}

}
