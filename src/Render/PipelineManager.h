#ifndef PIPELINE_MANAGER_H
#define PIPELINE_MANAGER_H

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <vulkan/vulkan_core.h>

enum class GraphicsPipelineType : uint8_t
{
	Opaque,
	MaxValue
};

struct VkCustomPipelineCreateInfo
{
	GraphicsPipelineType PipelineType;
	std::string VertexShaderName;
	std::string FragmentShaderName;
};

class PipelineManager final
{
public:
	PipelineManager();
	~PipelineManager();

	std::vector<std::string> GetAllPipelineNames() const;
	VkPipeline GetGraphicsPipeline(std::string PipelineName); 
	VkRenderPass GetRenderPass() const;
	VkPipelineLayout GetPileineLayout() const;

	VkResult CreateGraphicsPipeline(std::string PipelineName, const VkCustomPipelineCreateInfo* pCustomPipelineCreateInfo);

private:
	VkResult InitPipelineData();
	void DestroyAllGraphicsPipeline();

	VkResult CreateOpaqueGraphicsPipeline(std::string PipelineName, const VkCustomPipelineCreateInfo* pCustomPipelineCreateInfo);

	VkResult CreateRenderPass();

	void CreatePipelineVertexInput();
	void CreatePipelineMultySample();
	void CreatePipelineDynamicState();
	VkResult CreatePipelineLayout();

	VkPipelineLayout PipelineLayout = nullptr;
	VkPipelineVertexInputStateCreateInfo VertexInputInfo = { };
	VkPipelineMultisampleStateCreateInfo MultisampleInfo = { };
	
	std::vector<VkDynamicState> DynamicStates;

	VkRenderPass RenderPass = nullptr;
	std::unordered_map<std::string, VkPipeline> GraphicsPipelineMap;


};

#endif
