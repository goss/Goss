#include "gepch.h"
#include "Renderer2D.h"

#include "Core.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Texture.h"
#include "UniformBuffer.h"
#include "VertexArray.h"

namespace Goss
{
	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		float texIndex;
		float tilingFactor;

		int entityId;
	};

	struct CircleVertex
	{
		glm::vec3 worldPosition;
		glm::vec3 localPosition;
		glm::vec4 color;
		float thickness;
		float fade;

		int entityId;
	};

	struct LineVertex
	{
		glm::vec3 position;
		glm::vec4 color;

		int entityId;
	};

	struct Renderer2DData
	{
		static constexpr uint32_t MAX_QUADS = 20000;
		static constexpr uint32_t MAX_VERTICES = MAX_QUADS * 4;
		static constexpr uint32_t MAX_INDICES = MAX_QUADS * 6;
		static constexpr uint32_t MAX_TEXTURE_SLOTS = 32; // TODO: RenderCaps

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> quadShader;
		Ref<Texture2D> whiteTexture;

		Ref<VertexArray> circleVertexArray;
		Ref<VertexBuffer> circleVertexBuffer;
		Ref<Shader> circleShader;

		Ref<VertexArray> lineVertexArray;
		Ref<VertexBuffer> lineVertexBuffer;
		Ref<Shader> lineShader;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		uint32_t circleIndexCount = 0;
		CircleVertex* circleVertexBufferBase = nullptr;
		CircleVertex* circleVertexBufferPtr = nullptr;

		uint32_t lineVertexCount = 0;
		LineVertex* lineVertexBufferBase = nullptr;
		LineVertex* lineVertexBufferPtr = nullptr;

		float lineWidth = 2.0f;

		std::array<Ref<Texture2D>, MAX_TEXTURE_SLOTS> textureSlots;
		uint32_t textureSlotIndex = 1; // 0 = white texture

		glm::vec4 quadVertexPositions[4]{};

		Renderer2D::Statistics stats;

		struct CameraData
		{
			glm::mat4 viewProjection;
		};

		CameraData cameraBuffer{};
		Ref<UniformBuffer> cameraUniformBuffer;
	};

	static Renderer2DData data;

	void Renderer2D::Initialize()
	{
		data.quadVertexArray = VertexArray::Create();

		data.quadVertexBuffer = VertexBuffer::Create(Renderer2DData::MAX_VERTICES * sizeof(QuadVertex));
		data.quadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float, "a_TexIndex"},
			{ShaderDataType::Float, "a_TilingFactor"},
			{ShaderDataType::Int, "a_EntityID"}
		});

		data.quadVertexArray->AddVertexBuffer(data.quadVertexBuffer);
		data.quadVertexBufferBase = new QuadVertex[Renderer2DData::MAX_VERTICES];

		const auto quadIndices = new uint32_t[Renderer2DData::MAX_INDICES];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < Renderer2DData::MAX_INDICES; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		const Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, Renderer2DData::MAX_INDICES);
		data.quadVertexArray->SetIndexBuffer(quadIndexBuffer);
		delete[] quadIndices;

		// Circles
		data.circleVertexArray = VertexArray::Create();

		data.circleVertexBuffer = VertexBuffer::Create(Renderer2DData::MAX_VERTICES * sizeof(CircleVertex));
		data.circleVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_WorldPosition"},
			{ShaderDataType::Float3, "a_LocalPosition"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float, "a_Thickness"},
			{ShaderDataType::Float, "a_Fade"},
			{ShaderDataType::Int, "a_EntityID"}
		});
		data.circleVertexArray->AddVertexBuffer(data.circleVertexBuffer);
		data.circleVertexArray->SetIndexBuffer(quadIndexBuffer);
		data.circleVertexBufferBase = new CircleVertex[Renderer2DData::MAX_VERTICES];

		// Lines
		data.lineVertexArray = VertexArray::Create();

		data.lineVertexBuffer = VertexBuffer::Create(Renderer2DData::MAX_VERTICES * sizeof(LineVertex));
		data.lineVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Int, "a_EntityID"}
		});
		data.lineVertexArray->AddVertexBuffer(data.lineVertexBuffer);
		data.lineVertexBufferBase = new LineVertex[Renderer2DData::MAX_VERTICES];

		data.whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		data.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[Renderer2DData::MAX_TEXTURE_SLOTS]{};
		for (uint32_t i = 0; i < Renderer2DData::MAX_TEXTURE_SLOTS; i++)
		{
			samplers[i] = i;
		}

		data.quadShader = Shader::Create("Assets/Shaders/Renderer2D_Quad.glsl");
		data.circleShader = Shader::Create("Assets/Shaders/Renderer2D_Circle.glsl");
		data.lineShader = Shader::Create("Assets/Shaders/Renderer2D_Line.glsl");

		// Set first texture slot to 0
		data.textureSlots[0] = data.whiteTexture;

		data.quadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
		data.quadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
		data.quadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
		data.quadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};

		data.cameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::Shutdown()
	{
		delete[] data.quadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		data.cameraBuffer.viewProjection = camera.GetViewProjectionMatrix();
		data.cameraUniformBuffer->SetData(&data.cameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		data.cameraBuffer.viewProjection = camera.GetViewProjectionMatrix() * inverse(transform);
		data.cameraUniformBuffer->SetData(&data.cameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::StartBatch()
	{
		data.quadIndexCount = 0;
		data.quadVertexBufferPtr = data.quadVertexBufferBase;

		data.circleIndexCount = 0;
		data.circleVertexBufferPtr = data.circleVertexBufferBase;

		data.lineVertexCount = 0;
		data.lineVertexBufferPtr = data.lineVertexBufferBase;

		data.textureSlotIndex = 1;
	}

	void Renderer2D::Flush()
	{
		if (data.quadIndexCount)
		{
			const uint32_t dataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(data.quadVertexBufferPtr) - reinterpret_cast<uint8_t*>(data.
				quadVertexBufferBase));
			data.quadVertexBuffer->SetData(data.quadVertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < data.textureSlotIndex; i++)
				data.textureSlots[i]->Bind(i);

			data.quadShader->Bind();
			RenderCommand::DrawIndexed(data.quadVertexArray, data.quadIndexCount);
			data.stats.drawCalls++;
		}

		if (data.circleIndexCount)
		{
			const uint32_t dataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(data.circleVertexBufferPtr) - reinterpret_cast<uint8_t*>(data.
				circleVertexBufferBase));
			data.circleVertexBuffer->SetData(data.circleVertexBufferBase, dataSize);

			data.circleShader->Bind();
			RenderCommand::DrawIndexed(data.circleVertexArray, data.circleIndexCount);
			data.stats.drawCalls++;
		}

		if (data.lineVertexCount)
		{
			const uint32_t dataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(data.lineVertexBufferPtr) - reinterpret_cast<uint8_t*>(data.lineVertexBufferBase));
			data.lineVertexBuffer->SetData(data.lineVertexBufferBase, dataSize);

			data.lineShader->Bind();
			RenderCommand::SetLineWidth(data.lineWidth);
			RenderCommand::DrawLines(data.lineVertexArray, data.lineVertexCount);
			data.stats.drawCalls++;
		}
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({position.x, position.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		const glm::mat4 transform = translate(glm::mat4(1.0f), position)
			* scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
	                          const float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
	                          const float tilingFactor, const glm::vec4& tintColor)
	{
		const glm::mat4 transform = translate(glm::mat4(1.0f), position)
			* scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityId)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

		if (data.quadIndexCount >= Renderer2DData::MAX_INDICES)
			NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			constexpr float tilingFactor = 1.0f;
			constexpr float textureIndex = 0.0f;
			data.quadVertexBufferPtr->position = transform * data.quadVertexPositions[i];
			data.quadVertexBufferPtr->color = color;
			data.quadVertexBufferPtr->texCoord = textureCoords[i];
			data.quadVertexBufferPtr->texIndex = textureIndex;
			data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			data.quadVertexBufferPtr->entityId = entityId;
			data.quadVertexBufferPtr++;
		}

		data.quadIndexCount += 6;

		data.stats.quadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor,
	                          const glm::vec4& tintColor, int entityId)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

		if (data.quadIndexCount >= Renderer2DData::MAX_INDICES)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < data.textureSlotIndex; i++)
		{
			if (*data.textureSlots[i] == *texture)
			{
				textureIndex = static_cast<float>(i);
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (data.textureSlotIndex >= Renderer2DData::MAX_TEXTURE_SLOTS)
				NextBatch();

			textureIndex = static_cast<float>(data.textureSlotIndex);
			data.textureSlots[data.textureSlotIndex] = texture;
			data.textureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			data.quadVertexBufferPtr->position = transform * data.quadVertexPositions[i];
			data.quadVertexBufferPtr->color = tintColor;
			data.quadVertexBufferPtr->texCoord = textureCoords[i];
			data.quadVertexBufferPtr->texIndex = textureIndex;
			data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			data.quadVertexBufferPtr->entityId = entityId;
			data.quadVertexBufferPtr++;
		}

		data.quadIndexCount += 6;

		data.stats.quadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
	                                 const glm::vec4& color)
	{
		DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
	                                 const glm::vec4& color)
	{
		const glm::mat4 transform = translate(glm::mat4(1.0f), position)
			* rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f})
			* scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation,
	                                 const Ref<Texture2D>& texture, const float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation,
	                                 const Ref<Texture2D>& texture, const float tilingFactor, const glm::vec4& tintColor)
	{
		const glm::mat4 transform = translate(glm::mat4(1.0f), position)
			* rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f})
			* scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, const float thickness /*= 1.0f*/,
	                            const float fade /*= 0.005f*/, const int entityId /*= -1*/)
	{
		// TODO: implement for circles
		// if (data.QuadIndexCount >= Renderer2DData::MaxIndices)
		// 	NextBatch();

		for (auto& quadVertexPosition : data.quadVertexPositions)
		{
			data.circleVertexBufferPtr->worldPosition = transform * quadVertexPosition;
			data.circleVertexBufferPtr->localPosition = quadVertexPosition * 2.0f;
			data.circleVertexBufferPtr->color = color;
			data.circleVertexBufferPtr->thickness = thickness;
			data.circleVertexBufferPtr->fade = fade;
			data.circleVertexBufferPtr->entityId = entityId;
			data.circleVertexBufferPtr++;
		}

		data.circleIndexCount += 6;

		data.stats.quadCount++;
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& position, const glm::vec4& color, const int entityId)
	{
		data.lineVertexBufferPtr->position = p0;
		data.lineVertexBufferPtr->color = color;
		data.lineVertexBufferPtr->entityId = entityId;
		data.lineVertexBufferPtr++;

		data.lineVertexBufferPtr->position = position;
		data.lineVertexBufferPtr->color = color;
		data.lineVertexBufferPtr->entityId = entityId;
		data.lineVertexBufferPtr++;

		data.lineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const int entityId)
	{
		const auto a = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		const auto b = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		const auto c = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		const auto d = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(a, b, color, entityId);
		DrawLine(b, c, color, entityId);
		DrawLine(c, d, color, entityId);
		DrawLine(d, a, color, entityId);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityId)
	{
		glm::vec3 lineVertices[4]{};
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * data.quadVertexPositions[i];

		DrawLine(lineVertices[0], lineVertices[1], color, entityId);
		DrawLine(lineVertices[1], lineVertices[2], color, entityId);
		DrawLine(lineVertices[2], lineVertices[3], color, entityId);
		DrawLine(lineVertices[3], lineVertices[0], color, entityId);
	}

	float Renderer2D::GetLineWidth()
	{
		return data.lineWidth;
	}

	void Renderer2D::SetLineWidth(const float width)
	{
		data.lineWidth = width;
	}

	void Renderer2D::ResetStats()
	{
		memset(&data.stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return data.stats;
	}
}
