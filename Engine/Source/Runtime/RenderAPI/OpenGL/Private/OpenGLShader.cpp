#include "gepch.h"
#include "OpenGLShader.h"

#include "Core.h"

#include <glad/glad.h>
#include <gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "OpenGLUniformBuffer.h"

namespace Goss
{
	namespace Utils 
	{
		static const char* GetShaderCompiledDirectory()
		{
			return "Assets/Shaders/Compiled/OpenGL/";
		}

		static void CreateCompileDirectoryIfNeeded()
		{
			const std::string cacheDirectory = GetShaderCompiledDirectory();
			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}

		static const char* GLShaderStageToString(const GLuint stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
				case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
				default: ;
			}
			GE_CORE_ASSERT(false)
			return nullptr;
		}

		static const char* GLShaderStageFileExtension(const uint32_t stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:    return ".vert.spv";
				case GL_FRAGMENT_SHADER:  return ".frag.spv";
				default: ;
			}
			GE_CORE_ASSERT(false)
			return "";
		}
	}

	OpenGLShader::OpenGLShader(std::string name) : shaderName(std::move(name))
	{
		Utils::CreateCompileDirectoryIfNeeded();
		{
			std::string vertexSrc = shaderName;
			vertexSrc.append(Utils::GLShaderStageFileExtension(GL_VERTEX_SHADER));

			std::string fragmentSrc = shaderName;
			fragmentSrc.append(Utils::GLShaderStageFileExtension(GL_FRAGMENT_SHADER));

			const std::string vertSpv = ReadFile(vertexSrc);
			const std::string fragSpv = ReadFile(fragmentSrc);

			const std::unordered_map<GLuint, std::string> sources{{GL_VERTEX_SHADER, vertSpv}, {GL_FRAGMENT_SHADER, fragSpv}};

			GetShaderBinaries(sources);
			CreateProgram();
		}
	}

	OpenGLShader::OpenGLShader(std::string name, const std::string& vertexSrc, const std::string& fragmentSrc) : shaderName(std::move(name))
	{
		const std::unordered_map<GLuint, std::string> sources{{GL_VERTEX_SHADER, vertexSrc}, {GL_FRAGMENT_SHADER, fragmentSrc}};

		GetShaderBinaries(sources);
		CreateProgram();
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(programId);
	}

	std::string OpenGLShader::ReadFile(const std::string& name)
	{
		std::string location = Utils::GetShaderCompiledDirectory();
		location.append(name);

		std::string result;
		std::ifstream stream(location, std::ios::in | std::ios::binary); 
		if (stream)
		{
			stream.seekg(0, std::ios::end);
			const auto size = stream.tellg();
			if (size != -1)
			{
				result.resize(size);
				stream.seekg(0, std::ios::beg);
				stream.read(result.data(), size);
			}
			else
			{
				GE_CORE_ERROR("Could not read from file '{0}'", name);
			}
		}
		else
		{
			GE_CORE_ERROR("Could not open file '{0}'", name);
		}

		return result;
	}

	void OpenGLShader::GetShaderBinaries(const std::unordered_map<GLuint, std::string>& sources)
	{
		const std::filesystem::path directory = Utils::GetShaderCompiledDirectory();

		auto& shaderData = shaderSources;
		shaderData.clear();
		for (auto&& [stage, source] : sources)
		{
			std::filesystem::path path = directory / (shaderName + Utils::GLShaderStageFileExtension(stage));

			std::ifstream in(path, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read(reinterpret_cast<char*>(data.data()), size);
			}
		}

		for (auto&& [stage, data]: shaderSources)
		{
			Reflect(stage, data);
		}
	}

	void OpenGLShader::CreateProgram()
	{
		const GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : shaderSources)
		{
			GLuint shaderId = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderId, "main", 0, nullptr, nullptr);

			GLint compiled = 0;
			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
			if(compiled == GL_TRUE)
			{
				glAttachShader(program, shaderId);
			}
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			GE_CORE_ERROR("Shader linking failed ({0}):\n{1}", shaderName, infoLog.data());

			glDeleteProgram(program);

			for (const auto id : shaderIDs)
			{
				glDeleteShader(id);
			}
		}

		for (const auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		programId = program;
	}

	void OpenGLShader::Reflect(const GLuint stage, const std::vector<uint32_t>& shaderData)
	{
		const spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		GE_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), shaderName);
		GE_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
		GE_CORE_TRACE("    {0} resources", resources.sampled_images.size());

		GE_CORE_TRACE("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			auto bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			auto memberCount = bufferType.member_types.size();

			GE_CORE_TRACE("  {0}", resource.name);
			GE_CORE_TRACE("    Size = {0}", bufferSize);
			GE_CORE_TRACE("    Binding = {0}", binding);
			GE_CORE_TRACE("    Members = {0}", memberCount);
		}
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(programId);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, const int value)
	{
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, const uint32_t count)
	{
		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, const float value)
	{
		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		UploadUniformFloat2(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, const int value) const
	{
		const GLint location = glGetUniformLocation(programId, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, const int* values, const uint32_t count) const
	{
		const GLint location = glGetUniformLocation(programId, name.c_str());
		glUniform1iv(location, static_cast<GLsizei>(count), values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float value) const
	{
		const GLint location = glGetUniformLocation(programId, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) const
	{
		const GLint location = glGetUniformLocation(programId, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) const
	{
		const GLint location = glGetUniformLocation(programId, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) const
	{
		const GLint location = glGetUniformLocation(programId, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const
	{
		const GLint location = glGetUniformLocation(programId, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
	{
		const GLint location = glGetUniformLocation(programId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(matrix));
	}

	void OpenGLShader::SetColor(const glm::vec4& value)
	{
		const GLuint blockIndex = glGetUniformBlockIndex(programId, "UniformBuffer");

		GLint blockSize;
		glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
		glUniformBlockBinding(programId, blockIndex, 0);

		const GLfloat color[] = {value.x, value.y, value.z, value.w};
		GLubyte* blockBuffer = static_cast<GLubyte*>(malloc(blockSize));
		memcpy(blockBuffer, color, 16);

		unsigned int ubo;
		glGenBuffers(1, &ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER, 16, nullptr, GL_STATIC_DRAW); // allocate 16 bytes of memory
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);

		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 16, blockBuffer); 
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//const Ref<UniformBuffer> uniformBuffer = UniformBuffer::Create(blockSize, blockIndex);
		//uniformBuffer->SetData(blockBuffer, blockSize, 0);

		free(blockBuffer);
	}
}
