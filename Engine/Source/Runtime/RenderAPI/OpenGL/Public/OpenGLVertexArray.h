#pragma once

#include "VertexArray.h"

namespace Goss
{
	class OpenGLVertexArray final : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;

		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(const Ref<VertexBuffer>& buffer) override;
		void SetIndexBuffer(const Ref<IndexBuffer>& buffer) override;

		const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return vertexBuffers; }
		const Ref<IndexBuffer>& GetIndexBuffer() const override { return indexBuffer; }
	private:
		uint32_t rendererId;
		uint32_t vertexBufferIndex = 0;
		std::vector<Ref<VertexBuffer>> vertexBuffers;
		Ref<IndexBuffer> indexBuffer;
	};
}
