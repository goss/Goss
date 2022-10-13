#include "gepch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace Goss
{
	//const GLuint blockIndex = glGetUniformBlockIndex(programId, "UniformBuffer");
	//GLint blockSize;
	//glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
	//glUniformBlockBinding(programId, blockIndex, 0);

	OpenGLUniformBuffer::OpenGLUniformBuffer(const uint32_t size, const uint32_t binding)
	{
		glCreateBuffers(1, &rendererId);
		glNamedBufferData(rendererId, size, nullptr, GL_STATIC_DRAW);  //GL_STREAM_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, rendererId);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &rendererId);
		GE_CORE_INFO("DELETED UBO");
	}

	void OpenGLUniformBuffer::SetData(const void* data, const uint32_t size, const uint32_t offset)
	{
		glNamedBufferSubData(rendererId, offset, size, data);
	}
}
