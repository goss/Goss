#include "Pipeline.h"

#include <cassert>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <iostream>

namespace Goss
{
	Pipeline::Pipeline(EngineDevice& device, const char* vertFilepath, const char* fragFilepath, const PipelineConfigInfo& configInfo):
		engineDevice(device), graphicsPipeline(nullptr), vertShaderModule(nullptr), fragShaderModule(nullptr)
	{
		CreateGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
	}

	Pipeline::~Pipeline()
	{
		const VkDevice device = engineDevice.Device();
		vkDestroyShaderModule(device, vertShaderModule, nullptr);
		vkDestroyShaderModule(device, fragShaderModule, nullptr);
		vkDestroyPipeline(device, graphicsPipeline, nullptr);
	}

	void Pipeline::Bind(const VkCommandBuffer commandBuffer) const
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
	}

	PipelineConfigInfo Pipeline::DefaultPipelineConfigInfo(const uint32_t width, const uint32_t height)
	{
		PipelineConfigInfo configInfo{};
		configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		configInfo.viewport.x = 0.0f;
		configInfo.viewport.y = 0.0f;
		configInfo.viewport.width = static_cast<float>(width);
		configInfo.viewport.height = static_cast<float>(height);
		configInfo.viewport.minDepth = 0.0f;
		configInfo.viewport.maxDepth = 1.0f;

		configInfo.scissor.offset = {0, 0};
		configInfo.scissor.extent = {width, height};

		configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configInfo.viewportInfo.viewportCount = 1;
		configInfo.viewportInfo.pViewports = &configInfo.viewport;
		configInfo.viewportInfo.scissorCount = 1;
		configInfo.viewportInfo.pScissors = &configInfo.scissor;

		configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
		configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		configInfo.rasterizationInfo.lineWidth = 1.0f;
		configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
		configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
		configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
		configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

		configInfo.multiSampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo.multiSampleInfo.sampleShadingEnable = VK_FALSE;
		configInfo.multiSampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		configInfo.multiSampleInfo.minSampleShading = 1.0f;           // Optional
		configInfo.multiSampleInfo.pSampleMask = nullptr;             // Optional
		configInfo.multiSampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
		configInfo.multiSampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

		configInfo.colorBlendAttachment.colorWriteMask =VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
		configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
		configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

		configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
		configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
		configInfo.colorBlendInfo.attachmentCount = 1;
		configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
		configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
		configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
		configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
		configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

		configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
		configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
		configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
		configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
		configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
		configInfo.depthStencilInfo.front = {};  // Optional
		configInfo.depthStencilInfo.back = {};   // Optional

		return configInfo;
	}

	std::vector<char> Pipeline::ReadFile(const char* filepath)
	{
		if(std::ifstream file(filepath, std::ios::ate, std::ios::binary); file.is_open())
		{
			const std::streamsize fileSize = file.tellg();
			std::vector<char> buffer(fileSize);

			file.seekg(0);
			file.read(buffer.data(), fileSize);
			file.close();
			return buffer;
		}
		throw std::runtime_error(std::string("Failed to open file: ").append(filepath));
	}

	void Pipeline::CreateGraphicsPipeline(const char* vertFilepath, const char* fragFilepath, const PipelineConfigInfo& configInfo)
	{
		assert(configInfo.pipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
		assert(configInfo.renderPass != VK_NULL_HANDLE && "Cannot create graphics pipeline: no renderPass provided in configInfo");

		const std::vector<char> vertCode = ReadFile(vertFilepath);
		const std::vector<char> fragCode = ReadFile(fragFilepath);

		std::cout << "Vertex Shader Code Size: " << vertCode.size() << std::endl;
		std::cout << "Fragment Shader Code Size: " << fragCode.size() << std::endl;

		CreateShaderModule(vertCode, &vertShaderModule);
		CreateShaderModule(fragCode, &fragShaderModule);

		VkPipelineShaderStageCreateInfo shaderStages[2];
		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = vertShaderModule;
		shaderStages[0].pName = "main";
		shaderStages[0].flags = 0;
		shaderStages[0].pSpecializationInfo = nullptr;

		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = fragShaderModule;
		shaderStages[1].pName = "main";
		shaderStages[1].flags = 0;
		shaderStages[1].pSpecializationInfo = nullptr;

		VkPipelineVertexInputStateCreateInfo vertexInputState{};
		vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputState.vertexAttributeDescriptionCount = 0;
		vertexInputState.vertexBindingDescriptionCount = 0;
		vertexInputState.pVertexAttributeDescriptions = nullptr;
		vertexInputState.pVertexBindingDescriptions = nullptr;

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputState;
		pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
		pipelineInfo.pMultisampleState = &configInfo.multiSampleInfo;
		pipelineInfo.pViewportState = &configInfo.viewportInfo;
		pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
		pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
		pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
		pipelineInfo.pDynamicState = nullptr;

		pipelineInfo.layout = configInfo.pipelineLayout;
		pipelineInfo.renderPass = configInfo.renderPass;
		pipelineInfo.subpass = configInfo.subpass;

		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if(vkCreateGraphicsPipelines(engineDevice.Device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create graphics pipeline");
		}
	}

	void Pipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) const
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if(vkCreateShaderModule(engineDevice.Device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create shader module");
		}
	}
}
