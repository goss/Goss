#pragma once

#include "Core.h"
#include "Framebuffer.h"

namespace Goss
{
	class OpenGLFramebuffer final : public Framebuffer
	{
	public:
		OpenGLFramebuffer(FramebufferSpecification spec);
		~OpenGLFramebuffer() override;

		void Invalidate();

		void Bind() override;
		void Unbind() override;

		void Resize(uint32_t width, uint32_t height) override;
		int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		void ClearAttachment(uint32_t attachmentIndex, int value) override;

		uint32_t GetColorAttachmentRendererId(const uint32_t index = 0) const override { GE_CORE_ASSERT(index < colorAttachments.size()) return colorAttachments[index]; }

		const FramebufferSpecification& GetSpecification() const override { return specification; }
	private:
		uint32_t rendererId = 0;
		FramebufferSpecification specification;

		std::vector<FramebufferTextureSpecification> colorAttachmentSpecifications;
		FramebufferTextureSpecification depthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> colorAttachments;
		uint32_t depthAttachment = 0;
	};

}
