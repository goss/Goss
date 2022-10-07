#include "gepch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Goss
{
	OpenGLTexture2D::OpenGLTexture2D(const int width, const int height)
		: width(width), height(height)
	{
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &randererId);
		glTextureStorage2D(randererId, 1, internalFormat, width, height);

		glTextureParameteri(randererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(randererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(randererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(randererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(std::string path)
		: texturePath(std::move(path)), width(0), height(0), internalFormat(0), dataFormat(0)
	{
		int channels = 0;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);
		}

		if (data)
		{
			isLoaded = true;

			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			GE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!")

			glCreateTextures(GL_TEXTURE_2D, 1, &randererId);
			glTextureStorage2D(randererId, 1, internalFormat, width, height);

			glTextureParameteri(randererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(randererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(randererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(randererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(randererId, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &randererId);
	}

	void OpenGLTexture2D::SetData(void* data, const uint32_t size)
	{
		const uint32_t bpp = dataFormat == GL_RGBA ? 4 : 3;
		GE_CORE_ASSERT(size == width * height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(randererId, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(const uint32_t slot) const
	{
		glBindTextureUnit(slot, randererId);
	}
}
