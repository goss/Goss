#pragma once
#include "UniformBuffer.h"

namespace Goss
{
	class OpenGLUniformBuffer final : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		~OpenGLUniformBuffer() override;

		void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
	private:
		uint32_t rendererID = 0;
	};
}
