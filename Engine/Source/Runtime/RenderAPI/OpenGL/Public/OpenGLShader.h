#pragma once

#include "Shader.h"
#include <glm.hpp>

#include "UniformBuffer.h"
#include "glad/glad.h"

namespace Goss
{
	class OpenGLShader final : public Shader
	{
	public:
		explicit OpenGLShader(std::string name);
		OpenGLShader(std::string name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader() override;

		void Bind() const override;
		void Unbind() const override;

		/*Uniforms*/
		void SetInt(const std::string& name, int value) override;
		void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		void SetFloat(const std::string& name, float value) override;
		void SetFloat2(const std::string& name, const glm::vec2& value) override;
		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat4(const std::string& name, const glm::vec4& value) override;
		void SetMat4(const std::string& name, const glm::mat4& value) override;

		/*Uniform Buffers*/
		void SetColor(const glm::vec4& value) override;
		void SetTransformAndViewMatrix(const glm::mat4& viewProjection, const glm::mat4& position) override;

		const std::string& GetName() const override { return shaderName; }

private:
		void UploadUniformInt(const std::string& name, int value) const;
		void UploadUniformIntArray(const std::string& name, const int* values, uint32_t count) const;

		void UploadUniformFloat(const std::string& name, float value) const;
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value) const;
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value) const;
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value) const;

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

		GLint GetUniformLocation(const std::string& name) const;

		static std::string ReadFile(const std::string& name);

		void GetShaderBinaries(const std::unordered_map<GLuint, std::string>& sources);
		void CreateProgram();
		void Reflect(GLuint stage, const std::vector<uint32_t>& shaderData);

		mutable std::unordered_map<std::string, GLint> uniformLocations;

		uint32_t programId;
		std::string shaderName;

		std::unordered_map<GLuint, std::vector<uint32_t>> shaderSources;

		// TODO combine buffers?
		Ref<UniformBuffer> colorUniformBuffer; 
		Ref<UniformBuffer> matrixUniformBuffer;
	};

}
