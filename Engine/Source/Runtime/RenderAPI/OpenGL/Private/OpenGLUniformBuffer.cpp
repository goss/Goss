#include "gepch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace Goss
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(const uint32_t size, const uint32_t binding)
	{
		glCreateBuffers(1, &rendererId);
		glNamedBufferData(rendererId, size, nullptr, GL_DYNAMIC_DRAW); 
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, rendererId);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &rendererId);
	}

	void OpenGLUniformBuffer::SetData(const void* data, const uint32_t size, const uint32_t offset)
	{
		glNamedBufferSubData(rendererId, offset, size, data);
	}
}
