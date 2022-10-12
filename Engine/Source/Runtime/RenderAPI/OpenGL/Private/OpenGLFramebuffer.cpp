#include "gepch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

#include "Framebuffer.h"

namespace Goss
{
	static constexpr uint32_t MAX_FRAMEBUFFER_SIZE = 8192;

	namespace Utils
	{
		static GLenum TextureTarget(const bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(const bool multisampled, GLuint* outId, const GLsizei count)
		{
			glCreateTextures(TextureTarget(multisampled), count, outId);
		}

		static void BindTexture(const bool multisampled, const uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(const uint32_t id, const int samples, const GLenum internalFormat, const GLenum format, const GLsizei width, const GLsizei height, const int index)
		{
			const bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(internalFormat), width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(const uint32_t id, const int samples, const GLenum format, const GLenum attachmentType, const GLsizei width, const GLsizei height)
		{
			const bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

		static bool IsDepthFormat(const FramebufferTextureFormat format)
		{
			return format == FramebufferTextureFormat::Depth24Stencil8;
		}

		static GLenum TextureFormatToGL(const FramebufferTextureFormat format)
		{
			switch (format)
			{
				case FramebufferTextureFormat::RGBA8:      return GL_RGBA8;
				case FramebufferTextureFormat::RedInteger: return GL_RED_INTEGER;
				default: ;
			}

			GE_CORE_ASSERT(false)
			return 0;
		}

	}

	OpenGLFramebuffer::OpenGLFramebuffer(FramebufferSpecification spec)
		: specification(std::move(spec))
	{
		for (auto& textureSpecification : specification.attachments.attachments)
		{
			if (!Utils::IsDepthFormat(textureSpecification.textureFormat))
				colorAttachmentSpecifications.emplace_back(textureSpecification);
			else
				depthAttachmentSpecification = textureSpecification;
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &rendererId);
		glDeleteTextures(static_cast<GLsizei>(colorAttachments.size()), colorAttachments.data());
		glDeleteTextures(1, &depthAttachment);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (rendererId)
		{
			glDeleteFramebuffers(1, &rendererId);
			glDeleteTextures(static_cast<GLsizei>(colorAttachments.size()), colorAttachments.data());
			glDeleteTextures(1, &depthAttachment);
			
			colorAttachments.clear();
			depthAttachment = 0;
		}

		glCreateFramebuffers(1, &rendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, rendererId);

		const bool multisample = specification.samples > 1;

		// Attachments
		if (!colorAttachmentSpecifications.empty())
		{
			colorAttachments.resize(colorAttachmentSpecifications.size());
			const auto size = colorAttachments.size();
			Utils::CreateTextures(multisample, colorAttachments.data(), static_cast<GLsizei>(size));

			for (int i = 0; i < static_cast<int>(size); i++)
			{
				Utils::BindTexture(multisample, colorAttachments[i]);
				switch (colorAttachmentSpecifications[i].textureFormat)
				{
					case FramebufferTextureFormat::RGBA8:
						Utils::AttachColorTexture(colorAttachments[i], static_cast<int>(specification.samples), GL_RGBA8, GL_RGBA, static_cast<GLsizei>(specification.width), static_cast<GLsizei>(specification.height), i);
						break;
					case FramebufferTextureFormat::RedInteger:
						Utils::AttachColorTexture(colorAttachments[i], static_cast<int>(specification.samples), GL_R32I, GL_RED_INTEGER, static_cast<GLsizei>(specification.width), static_cast<GLsizei>(specification.height), i);
						break;
					default: ;
				}
			}
		}

		if (depthAttachmentSpecification.textureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multisample, &depthAttachment, 1);
			Utils::BindTexture(multisample, depthAttachment);
			switch (depthAttachmentSpecification.textureFormat)
			{
				case FramebufferTextureFormat::Depth24Stencil8:
					Utils::AttachDepthTexture(depthAttachment, static_cast<int>(specification.samples), GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, static_cast<GLsizei>(specification.width), static_cast<GLsizei>(specification.height));
					break;
				default: ;
			}
		}

		if (colorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}
		else
		{
			const size_t size = colorAttachments.size();
			GE_CORE_ASSERT(size <= 4)
			constexpr GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(static_cast<GLsizei>(size), buffers);
		}

		GE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!")

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rendererId);
		glViewport(0, 0, static_cast<GLsizei>(specification.width), static_cast<GLsizei>(specification.height));
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > MAX_FRAMEBUFFER_SIZE || height > MAX_FRAMEBUFFER_SIZE)
		{
			GE_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
			return;
		}
		specification.width = width;
		specification.height = height;
		
		Invalidate();
	}

	int OpenGLFramebuffer::ReadPixel(const uint32_t attachmentIndex, const int x, int y)
	{
		GE_CORE_ASSERT(attachmentIndex < colorAttachments.size())

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFramebuffer::ClearAttachment(const uint32_t attachmentIndex, const int value)
	{
		GE_CORE_ASSERT(attachmentIndex < colorAttachments.size())

		const auto& spec = colorAttachmentSpecifications[attachmentIndex];
		glClearTexImage(colorAttachments[attachmentIndex], 0, Utils::TextureFormatToGL(spec.textureFormat), GL_INT, &value);
	}

}
