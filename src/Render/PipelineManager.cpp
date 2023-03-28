#include "PipelineManager.h"
#include "RenderContext.h"
#include "RenderUtil.h"
#include "ShaderManager.h"
#include <vector>

PipelineManager::PipelineManager()
{
	InitPipelineData();	
}

PipelineManager::~PipelineManager()
{
	DestroyAllGraphicsPipeline();
}

std::vector<std::string> PipelineManager::GetAllPipelineNames() const
{
	std::vector<std::string> Names;
	for(auto& PipelineInfo : GraphicsPipelineMap)
	{
		Names.push_back(PipelineInfo.first);
	}
	return Names;
}

VkPipeline PipelineManager::GetGraphicsPipeline(std::string PipelineName)
{
	if(GraphicsPipelineMap.count(PipelineName))
		return GraphicsPipelineMap[PipelineName];
	return nullptr;
}

VkRenderPass PipelineManager::GetRenderPass() const
{
	return RenderPass;
}

VkPipelineLayout PipelineManager::GetPileineLayout() const
{
	return PipelineLayout;
}

VkResult PipelineManager::InitPipelineData()
{
	CreatePipelineVertexInput();
	CreatePipelineMultySample();
	CreatePipelineDynamicState();
	VkResult VR = CreatePipelineLayout();
	if(VR != VK_SUCCESS)
		return VR;
	VR = CreateRenderPass();
	if(VR != VK_SUCCESS)
		return VR;

	return VK_SUCCESS;
}

VkResult PipelineManager::CreateGraphicsPipeline(std::string PipelineName, const VkCustomPipelineCreateInfo* pCustomPipelineCreateInfo)
{
	switch (pCustomPipelineCreateInfo->PipelineType) 
	{
		case GraphicsPipelineType::Opaque:
			return CreateOpaqueGraphicsPipeline(PipelineName, pCustomPipelineCreateInfo);
		default:
			break;
	}

	return VK_ERROR_UNKNOWN;
}

VkResult PipelineManager::CreateOpaqueGraphicsPipeline(std::string PipelineName, const VkCustomPipelineCreateInfo* pCustomPipelineCreateInfo)
{
	if(GraphicsPipelineMap.count(PipelineName))
		return VK_ERROR_UNKNOWN;

	/*** Vertex Input ***/
	/* CreatePipelineVertexInput() */

	/*** Vertex Assemble ***/
	VkPipelineInputAssemblyStateCreateInfo VertexAssembleInfo = { };
	VertexAssembleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	VertexAssembleInfo.primitiveRestartEnable = VK_FALSE;
	VertexAssembleInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	/*** Shader Vertex ***/
	std::vector<VkPipelineShaderStageCreateInfo> ShaderStageInfos;
	ShaderStageInfos.push_back(RenderContext::Get().Shader->GetPipelineShaderStageInfo(ShaderType::VertexShader, pCustomPipelineCreateInfo->VertexShaderName));

	/*** Viewport(Dynamic) ***/
	VkViewport Viewport = { }; 
	VkRect2D ScissorRect = { };
	VkPipelineViewportStateCreateInfo ViewportInfo = { };
	ViewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	ViewportInfo.viewportCount = 1;
	ViewportInfo.pViewports = &Viewport;
	ViewportInfo.scissorCount = 1;
	ViewportInfo.pScissors = &ScissorRect;

	/*** MultySample ***/
	/* CreatePipelineMultySample() */

	/*** DepthStencil Test ***/
	VkPipelineDepthStencilStateCreateInfo DepthStencilInfo = { };
	DepthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	DepthStencilInfo.depthTestEnable = VK_TRUE;
	DepthStencilInfo.depthWriteEnable = VK_TRUE;
	DepthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	DepthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	DepthStencilInfo.minDepthBounds = 0.f;
	DepthStencilInfo.maxDepthBounds = 1.f;
	DepthStencilInfo.stencilTestEnable = VK_FALSE;
	DepthStencilInfo.front = { };
	DepthStencilInfo.back = { };

	/*** Rasterization ***/
	VkPipelineRasterizationStateCreateInfo RasterizationInfo = { };
	RasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	RasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
	RasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	RasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	RasterizationInfo.lineWidth = 1;
	RasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;

	/*** Shader Fragment ***/
	ShaderStageInfos.push_back(RenderContext::Get().Shader->GetPipelineShaderStageInfo(ShaderType::FragmentShader, pCustomPipelineCreateInfo->FragmentShaderName));

	/*** Color Blend ***/
	VkPipelineColorBlendAttachmentState ColorBlendState = { };
	ColorBlendState.blendEnable = VK_FALSE;
	ColorBlendState.colorWriteMask = VK_COLOR_COMPONENT_A_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_R_BIT;

	VkPipelineColorBlendStateCreateInfo ColorBlendInfo = { };
	ColorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	ColorBlendInfo.logicOpEnable = VK_FALSE;
	ColorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
	ColorBlendInfo.attachmentCount = 1;
	ColorBlendInfo.pAttachments = &ColorBlendState;

	/*** Set DynamicState ***/
	/* CreatePipelineDynamicState() */
	VkPipelineDynamicStateCreateInfo DynamicStateInfo = { };
	DynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	DynamicStateInfo.dynamicStateCount = DynamicStates.size();
	DynamicStateInfo.pDynamicStates = DynamicStates.data();

	/*** Layout ***/
	/* CreatePipelineLayout */

	/*** RenderPass ***/
	/* CreateRenderPass */

	/*** Create Pipeline ***/
	GraphicsPipelineMap[PipelineName] = nullptr;
	VkGraphicsPipelineCreateInfo GraphicsPipelineInfo = { };
	GraphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	GraphicsPipelineInfo.pVertexInputState = &VertexInputInfo;
	GraphicsPipelineInfo.pInputAssemblyState = &VertexAssembleInfo;
	GraphicsPipelineInfo.stageCount = ShaderStageInfos.size();
	GraphicsPipelineInfo.pStages = ShaderStageInfos.data();
	GraphicsPipelineInfo.pTessellationState = nullptr;
	GraphicsPipelineInfo.pViewportState = &ViewportInfo;
	GraphicsPipelineInfo.pMultisampleState = &MultisampleInfo;
	GraphicsPipelineInfo.pRasterizationState = &RasterizationInfo;
	GraphicsPipelineInfo.pDepthStencilState = &DepthStencilInfo;
	GraphicsPipelineInfo.pColorBlendState = &ColorBlendInfo;
	GraphicsPipelineInfo.pDynamicState = &DynamicStateInfo;
	GraphicsPipelineInfo.layout = PipelineLayout;
	GraphicsPipelineInfo.renderPass = RenderPass;
	GraphicsPipelineInfo.subpass = 0;
	GraphicsPipelineInfo.basePipelineHandle = nullptr;
	GraphicsPipelineInfo.basePipelineIndex = 0;

	return vkCreateGraphicsPipelines(RenderContext::Get().LogicDevice, nullptr, 1, &GraphicsPipelineInfo, nullptr, &(GraphicsPipelineMap[PipelineName]));
}

void PipelineManager::DestroyAllGraphicsPipeline()
{
	// DestroyPipeline
	for(auto& PipelineInfo : GraphicsPipelineMap)
	{
		vkDestroyPipeline(RenderContext::Get().LogicDevice, PipelineInfo.second, nullptr);
	}
	GraphicsPipelineMap.clear();

	vkDestroyRenderPass(RenderContext::Get().LogicDevice, RenderPass, nullptr);
	vkDestroyPipelineLayout(RenderContext::Get().LogicDevice, PipelineLayout, nullptr);
}

VkResult PipelineManager::CreateRenderPass()
{
	/*** RenderPass ***/
	/* Attachment */
	std::vector<VkAttachmentDescription> RenderAttachmentDescs;

	VkAttachmentDescription RenderTargetAttachmentDesc = { };
	RenderTargetAttachmentDesc.format = RenderContext::Get().RenderFormat.ImageFormat;
	RenderTargetAttachmentDesc.samples = VK_SAMPLE_COUNT_1_BIT;
	RenderTargetAttachmentDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	RenderTargetAttachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	RenderTargetAttachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	RenderTargetAttachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	RenderTargetAttachmentDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	RenderTargetAttachmentDesc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	RenderAttachmentDescs.push_back(std::move(RenderTargetAttachmentDesc));

	VkAttachmentDescription DepthStencilAttachmentDesc = { };
	DepthStencilAttachmentDesc.format = RenderContext::Get().RenderFormat.DepthStencilFormat;
	DepthStencilAttachmentDesc.samples = VK_SAMPLE_COUNT_1_BIT;
	DepthStencilAttachmentDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	DepthStencilAttachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
   	DepthStencilAttachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	DepthStencilAttachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	DepthStencilAttachmentDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	DepthStencilAttachmentDesc.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	RenderAttachmentDescs.push_back(std::move(DepthStencilAttachmentDesc));

	VkAttachmentReference RenderTargetAttachmentRef = { };
	RenderTargetAttachmentRef.attachment = 0;
	RenderTargetAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference DepthStencilAttachmentRef = { };
	DepthStencilAttachmentRef.attachment = 1;
	DepthStencilAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	/* SubPass(设置SubPass依赖的附件) */
	VkSubpassDescription RenderSubpassDesc = { };
	RenderSubpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	RenderSubpassDesc.inputAttachmentCount = 0;
	RenderSubpassDesc.pInputAttachments = nullptr;
	RenderSubpassDesc.colorAttachmentCount = 1;
	RenderSubpassDesc.pColorAttachments = &RenderTargetAttachmentRef;
	RenderSubpassDesc.pResolveAttachments = nullptr;
	RenderSubpassDesc.pDepthStencilAttachment = &DepthStencilAttachmentRef;
	RenderSubpassDesc.preserveAttachmentCount = 0;
	RenderSubpassDesc.pPreserveAttachments = nullptr;

	/* SubPass Dependency(决定SubPass执行顺序) */
	VkSubpassDependency SubPassDependency = { };
	SubPassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	SubPassDependency.dstSubpass = 0; //索引为0的SubPass
	SubPassDependency.srcAccessMask = VK_ACCESS_NONE;
	SubPassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	SubPassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	SubPassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

	/* Create RenderPass */
	VkRenderPassCreateInfo RenderPassInfo = { };
	RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	RenderPassInfo.attachmentCount = RenderAttachmentDescs.size();
	RenderPassInfo.pAttachments = RenderAttachmentDescs.data();
	RenderPassInfo.subpassCount = 1;
	RenderPassInfo.pSubpasses = &RenderSubpassDesc;
	RenderPassInfo.dependencyCount = 1;
	RenderPassInfo.pDependencies = &SubPassDependency;

	return vkCreateRenderPass(RenderContext::Get().LogicDevice, &RenderPassInfo, nullptr, &RenderPass);

}

void PipelineManager::CreatePipelineVertexInput()
{
	VertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;	
	VertexInputInfo.vertexBindingDescriptionCount = 1;
	VertexInputInfo.pVertexBindingDescriptions = &Vertex::GetVertexInputBinding();
	VertexInputInfo.vertexAttributeDescriptionCount = Vertex::GetVertexInputAttribute().size();
   	VertexInputInfo.pVertexAttributeDescriptions = Vertex::GetVertexInputAttribute().data();	
}

void PipelineManager::CreatePipelineMultySample()
{
	/*** MultySample ***/
	MultisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	MultisampleInfo.sampleShadingEnable = VK_FALSE;
	MultisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
}

void PipelineManager::CreatePipelineDynamicState()
{
	DynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
	DynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);
}

VkResult PipelineManager::CreatePipelineLayout()
{
	/*** Layout ***/
	/* Uniform */
	VkDescriptorSetLayoutCreateInfo UniformSetLayoutInfo = { };
	UniformSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	UniformSetLayoutInfo.bindingCount = 1;
	UniformSetLayoutInfo.pBindings = &Uniform::UniformBinding();
	
	std::vector<VkDescriptorSetLayout> SetLayouts;
	VkDescriptorSetLayout UniformSetLayout;
	VkResult VR = vkCreateDescriptorSetLayout(RenderContext::Get().LogicDevice, &UniformSetLayoutInfo, nullptr, &UniformSetLayout);
	if(VR != VK_SUCCESS)
		return VR;
	
	SetLayouts.push_back(std::move(UniformSetLayout));

	VkPushConstantRange ConstantRange = PushConstant::ConstansBinding();

	VkPipelineLayoutCreateInfo PipelineLayoutInfo = { };
	PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	PipelineLayoutInfo.setLayoutCount = SetLayouts.size();
	PipelineLayoutInfo.pSetLayouts = SetLayouts.data();
	PipelineLayoutInfo.pushConstantRangeCount = 1;
	PipelineLayoutInfo.pPushConstantRanges = &ConstantRange; 
	
	VR = vkCreatePipelineLayout(RenderContext::Get().LogicDevice, &PipelineLayoutInfo, nullptr, &PipelineLayout);
	if(VR != VK_SUCCESS)
		return VR;

	for(auto& SetLayout : SetLayouts)
		vkDestroyDescriptorSetLayout(RenderContext::Get().LogicDevice, SetLayout, nullptr);
	return VK_SUCCESS;
}
