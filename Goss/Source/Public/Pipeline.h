#pragma once
#include "EngineDevice.h"

// std
#include <vector>

namespace Goss
{
	struct PipelineConfigInfo
	{
		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multiSampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class Pipeline
	{
	public:
		Pipeline(EngineDevice& device, const char* vertFilepath, const char* fragFilepath, const PipelineConfigInfo& configInfo);
		~Pipeline();

		Pipeline(const Pipeline&) = delete;
		void operator=(const Pipeline&) = delete;

		void Bind(VkCommandBuffer commandBuffer) const;

		static PipelineConfigInfo DefaultPipelineConfigInfo(uint32_t width, uint32_t height);

	private:
		static std::vector<char> ReadFile(const char* filepath);
		void CreateGraphicsPipeline(const char* vertFilepath, const char* fragFilepath, const PipelineConfigInfo& configInfo);

		void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) const;

		EngineDevice& engineDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
}
