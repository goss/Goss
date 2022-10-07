#pragma once

#include "Shader.h"
#include <glm.hpp>

// TODO: REMOVE!
typedef unsigned int GLenum;

namespace Goss
{
	class OpenGLShader final : public Shader
	{
	public:
		explicit OpenGLShader(std::string filepath);
		OpenGLShader(std::string name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader() override;

		void Bind() const override;
		void Unbind() const override;

		void SetInt(const std::string& name, int value) override;
		void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		void SetFloat(const std::string& name, float value) override;
		void SetFloat2(const std::string& name, const glm::vec2& value) override;
		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat4(const std::string& name, const glm::vec4& value) override;
		void SetMat4(const std::string& name, const glm::mat4& value) override;

		const std::string& GetName() const override { return shaderName; }

		void UploadUniformInt(const std::string& name, int value) const;
		void UploadUniformIntArray(const std::string& name, const int* values, uint32_t count) const;

		void UploadUniformFloat(const std::string& name, float value) const;
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value) const;
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value) const;
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value) const;

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;
	private:
		static std::string ReadFile(const std::string& filepath);
		static std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);

		void CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources);
		void CompileOrGetOpenGLBinaries();
		void CreateProgram();
		void Reflect(GLenum stage, const std::vector<uint32_t>& shaderData);

		uint32_t rendererId;
		std::string filePath;
		std::string shaderName;

		std::unordered_map<GLenum, std::vector<uint32_t>> vulkanSpirv;
		std::unordered_map<GLenum, std::vector<uint32_t>> openGLSpirv;

		std::unordered_map<GLenum, std::string> openGLSourceCode;
	};

}
