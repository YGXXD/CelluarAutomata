#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

enum ShaderType : uint8_t
{
	VertexShader,
	FragmentShader,
	MaxValue
};

class ShaderManager final
{
public:
	ShaderManager();
	~ShaderManager();

	std::vector<std::string> GetAllShaderNames(ShaderType Type) const;
	VkResult CreateShader(ShaderType ShaterTypeEnum, std::string ShaderName, std::string ShaderPath);
	VkPipelineShaderStageCreateInfo GetPipelineShaderStageInfo(ShaderType ShaderTypeEnum, std::string ShaderName);
private:
	void DestroyAllShader();

	std::unordered_map<std::string, VkShaderModule> VertShaderMap;
	std::unordered_map<std::string, VkShaderModule> FragShaderMap;
};

class ShaderResource final
{
public:
	ShaderResource() = default;
	ShaderResource(ShaderResource& Copy) = delete;
	ShaderResource& operator=(ShaderResource& Copy) = delete;
	~ShaderResource() = default;

	VkDescriptorSet GetDescriptorSet() const;
	VkResult CreateShaderResource(VkPhysicalDevice PhysicalDevice, VkDevice LogicDevice);
	void DestroyShaderResource(VkDevice LogicDevice);
	void UploadUniformBuffer(void* pUniformBuffer);
private:
	VkResult CreatePipelineDescriptor(VkDevice LogicDevice);
	VkResult CreateUniformBuffer(VkPhysicalDevice PhysicalDevice, VkDevice LogicDevice);
	void UpdateDescriptor(VkDevice LogicDevice);
		
	VkDescriptorPool DescriptorPool = nullptr;
	VkDescriptorSetLayout DescriptorSetLayout = nullptr;
	VkDescriptorSet DescriptorSet = nullptr;

	VkBuffer UniformBuffer = nullptr;
	VkDeviceMemory UniformMemory = nullptr;
	void* UniformMapPtr = nullptr;
};

#endif
