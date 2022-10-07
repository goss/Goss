#pragma once

#include "OpenGLShader.h"
#include "Texture.h"

namespace Goss
{
	class OpenGLTexture2D final : public Texture2D
	{
	public:
		OpenGLTexture2D(int width, int height);
		OpenGLTexture2D(std::string path);
		~OpenGLTexture2D() override;

		uint32_t GetWidth() const override { return width;  }
		uint32_t GetHeight() const override { return height; }
		uint32_t GetRendererId() const override { return randererId; }

		const std::string& GetPath() const override { return texturePath; }

		void SetData(void* data, uint32_t size) override;

		void Bind(uint32_t slot = 0) const override;

		bool IsLoaded() const override { return isLoaded; }

		bool operator==(const Texture& other) const override
		{
			return randererId == other.GetRendererId();
		}
	private:
		std::string texturePath;
		bool isLoaded = false;
		int width, height;
		uint32_t randererId;
		GLenum internalFormat, dataFormat;
	};

}
