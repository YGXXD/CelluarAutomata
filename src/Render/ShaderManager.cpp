#include "ShaderManager.h"
#include "RenderContext.h"
#include "RenderUtil.h"
#include <cstring>
#include <fstream>
#include <vulkan/vulkan_core.h>

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	DestroyAllShader();
}

std::vector<std::string> ShaderManager::GetAllShaderNames(ShaderType Type) const
{
	std::vector<std::string> Names;
	const std::unordered_map<std::string, VkShaderModule>* pShaderMap = nullptr;
	switch(Type) 
	{
		case ShaderType::VertexShader:
			pShaderMap = &VertShaderMap;
			break;
		case ShaderType::FragmentShader:
			pShaderMap = &FragShaderMap;
			break;
		default:
			break;
	}
	if(pShaderMap)
	{
		for(auto& ShaderInfo : (*pShaderMap))
		{
			Names.push_back(ShaderInfo.first);
		}
	}

	return Names;
}

VkResult ShaderManager::CreateShader(ShaderType ShaderTypeEnum, std::string ShaderName, std::string ShaderPath)
{
	VkResult VR = VK_ERROR_UNKNOWN; 
 	std::ifstream ShaderFile(ShaderPath, std::ios::ate | std::ios::binary);
	if(ShaderFile.is_open())
	{
		uint32_t CodeSize = ShaderFile.tellg();
		ShaderFile.seekg(0);
		char CodeBuffer[CodeSize];
		ShaderFile.read(CodeBuffer, CodeSize);
		
		VkShaderModuleCreateInfo ShaderModuleInfo = { };
		ShaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		ShaderModuleInfo.pCode = (uint32_t*)CodeBuffer;
		ShaderModuleInfo.codeSize = CodeSize;

		std::unordered_map<std::string, VkShaderModule>* ShaderMap = nullptr;
		switch (ShaderTypeEnum)
		{
		case ShaderType::VertexShader:
			ShaderMap = &VertShaderMap;
			break;
		case ShaderType::FragmentShader:
			ShaderMap = &FragShaderMap;
			break;
		default:
			break;
		}

		if(ShaderMap && !ShaderMap->count(ShaderName))
		{
			(*ShaderMap)[ShaderName] = nullptr;
			VR = vkCreateShaderModule(RenderContext::Get().LogicDevice, &ShaderModuleInfo, nullptr, &(*ShaderMap)[ShaderName]);
		}

		ShaderFile.close();
	}

	return VR;	
}

VkPipelineShaderStageCreateInfo ShaderManager::GetPipelineShaderStageInfo(ShaderType ShaderTypeEnum, std::string ShaderName)
{
	VkPipelineShaderStageCreateInfo ShaderStageInfo = { };
	VkShaderStageFlagBits ShaderStageFlag = { };

	std::unordered_map<std::string, VkShaderModule>* ShaderMap = nullptr;
	switch (ShaderTypeEnum)
	{
	case ShaderType::VertexShader:
		ShaderMap = &VertShaderMap;
		ShaderStageFlag = VK_SHADER_STAGE_VERTEX_BIT;
		break;
	case ShaderType::FragmentShader:
		ShaderMap = &FragShaderMap;
		ShaderStageFlag = VK_SHADER_STAGE_FRAGMENT_BIT;
		break;
	default:
		break;
	}	

	if(ShaderMap && ShaderMap->count(ShaderName))
	{
		ShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		ShaderStageInfo.pName = "main";
		ShaderStageInfo.module = (*ShaderMap)[ShaderName];
		ShaderStageInfo.stage = ShaderStageFlag;
	}

	return ShaderStageInfo;
}

void ShaderManager::DestroyAllShader()
{
	for(auto& ShaderModuleInfo : VertShaderMap)
	{
		vkDestroyShaderModule(RenderContext::Get().LogicDevice, ShaderModuleInfo.second, nullptr);
	}
	VertShaderMap.clear();

	for(auto& ShaderModuleInfo : FragShaderMap)
	{
		vkDestroyShaderModule(RenderContext::Get().LogicDevice, ShaderModuleInfo.second, nullptr);
	}
	FragShaderMap.clear();
}

/* ##################################################### */

VkDescriptorSet ShaderResource::GetDescriptorSet() const
{
	return DescriptorSet;
}

void ShaderResource::UploadUniformBuffer(void *pUniformBuffer)
{
	memcpy(UniformMapPtr, pUniformBuffer, sizeof(Uniform));
}

VkResult ShaderResource::CreateShaderResource(VkPhysicalDevice PhysicalDevice, VkDevice LogicDevice)
{
	VkResult VR = CreatePipelineDescriptor(LogicDevice);
	if(VR != VK_SUCCESS)
		return VR;

	VR = CreateUniformBuffer(PhysicalDevice, LogicDevice);
	if(VR != VK_SUCCESS)
		return VR;

	UpdateDescriptor(LogicDevice);
	return VK_SUCCESS;
}

void ShaderResource::DestroyShaderResource(VkDevice LogicDevice)
{
	// Destroy UniformBuffer
	vkUnmapMemory(LogicDevice, UniformMemory);
	vkFreeMemory(LogicDevice, UniformMemory, nullptr);
	vkDestroyBuffer(LogicDevice, UniformBuffer, nullptr);

	// Destroy Descriptor
	vkDestroyDescriptorSetLayout(LogicDevice, DescriptorSetLayout, nullptr);
	vkDestroyDescriptorPool(LogicDevice, DescriptorPool, nullptr);
	UniformMapPtr = nullptr;
}

VkResult ShaderResource::CreatePipelineDescriptor(VkDevice LogicDevice)
{
	// Create DescriptorPool
	VkDescriptorPoolSize UniformPoolSize = { };
	UniformPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
   	UniformPoolSize.descriptorCount = 1;	

	VkDescriptorPoolCreateInfo DescriptorPoolInfo = { };
	DescriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	DescriptorPoolInfo.maxSets = 1;
	DescriptorPoolInfo.poolSizeCount = 1;
	DescriptorPoolInfo.pPoolSizes = &UniformPoolSize;
	
	VkResult VR = vkCreateDescriptorPool(LogicDevice, &DescriptorPoolInfo, nullptr, &DescriptorPool);	
	if(VR != VK_SUCCESS)
		return VR;

	// Create DescriptorSetLayout
	VkDescriptorSetLayoutBinding UniformSetLayBinding = { };
	UniformSetLayBinding.binding = 0;
	UniformSetLayBinding.descriptorCount = 1;
	UniformSetLayBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	UniformSetLayBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo = { };
	DescriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	DescriptorSetLayoutInfo.bindingCount = 1;
	DescriptorSetLayoutInfo.pBindings = &UniformSetLayBinding;

	VR = vkCreateDescriptorSetLayout(LogicDevice, &DescriptorSetLayoutInfo, nullptr, &DescriptorSetLayout);
	if(VR != VK_SUCCESS)
		return VR;

	VkDescriptorSetAllocateInfo DescriptorSetInfo = { };
	DescriptorSetInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	DescriptorSetInfo.descriptorSetCount = 1;
	DescriptorSetInfo.descriptorPool = DescriptorPool;
	DescriptorSetInfo.pSetLayouts = &DescriptorSetLayout;

	VR = vkAllocateDescriptorSets(LogicDevice, &DescriptorSetInfo, &DescriptorSet);	
	if(VR != VK_SUCCESS)
		return VR;

	return VK_SUCCESS;
}

VkResult ShaderResource::CreateUniformBuffer(VkPhysicalDevice PhysicalDevice, VkDevice LogicDevice)
{
	VkResult VR = VulkanUtil::CreateBuffer(PhysicalDevice, LogicDevice, sizeof(Uniform), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &UniformBuffer, &UniformMemory);
	if(VR != VK_SUCCESS)
		return VR;

	VR = vkMapMemory(LogicDevice, UniformMemory, 0, sizeof(Uniform), 0, &UniformMapPtr);
	if(VR != VK_SUCCESS)
		return VR;

	return VK_SUCCESS;
}

void ShaderResource::UpdateDescriptor(VkDevice LogicDevice)
{
	VkDescriptorBufferInfo UniformBufferInfo;
	UniformBufferInfo.buffer = UniformBuffer;
	UniformBufferInfo.offset = 0;
	UniformBufferInfo.range = sizeof(Uniform);
	
	VkWriteDescriptorSet WriteSet = { };
	WriteSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	WriteSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	WriteSet.pBufferInfo = &UniformBufferInfo;
	WriteSet.dstBinding = 0;
	WriteSet.dstSet = DescriptorSet;
	WriteSet.dstArrayElement = 0;
	WriteSet.descriptorCount = 1;

	vkUpdateDescriptorSets(LogicDevice, 1, &WriteSet, 0, nullptr);
}


