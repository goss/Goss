#pragma once

#include "Buffer.h"
#include "glad/glad.h"

namespace Goss
{
	class OpenGLVertexBuffer final : public VertexBuffer
	{
	public:
		explicit OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(const float* vertices, uint32_t size);
		~OpenGLVertexBuffer() override;

		void Bind() const override;
		void Unbind() const override;

		void SetData(const void* data, uint32_t size) override;

		const BufferLayout& GetLayout() const override { return bufferLayout; }
		void SetLayout(const BufferLayout& layout) override { bufferLayout = layout; }
	private:
		uint32_t rendererId;
		BufferLayout bufferLayout;
	};

	class OpenGLIndexBuffer final : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer() override;

		void Bind() const override;
		void Unbind() const override;

		uint32_t GetCount() const override { return count; }
	private:
		uint32_t rendererId;
		uint32_t count;
	};
}
