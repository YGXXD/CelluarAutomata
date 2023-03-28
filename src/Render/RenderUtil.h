#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include "../Math/Matrix4X4.h"

#define CheckVR(x)\
{\
	VkResult Result = (x);\
	if(Result != VK_SUCCESS)\
	{\
		std::stringstream ErrorStream;\
		std::string Error;\
		ErrorStream << #x <<" is Failed,In File:" << __FILE__ << ",In Line:" << __LINE__;\
		while(ErrorStream)\
		{\
			std::string SubStr;\
			ErrorStream >> SubStr;\
			Error += SubStr;\
			Error.push_back(' ');\
		}\
		throw Error.c_str();\
	}\
}\

struct Vertex 
{
	Vector3 Position;
	Vector3 Normal;

	static VkVertexInputBindingDescription& GetVertexInputBinding()
	{
		static VkVertexInputBindingDescription VertexInputBinding = 
		{
			0,
			sizeof(Vertex),
			VK_VERTEX_INPUT_RATE_VERTEX
	   	};
		
		return VertexInputBinding;
	}
	
	static std::vector<VkVertexInputAttributeDescription>& GetVertexInputAttribute()
	{
		static std::vector<VkVertexInputAttributeDescription> VertexInputAttributes = 
		{
			{0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0},
			{1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, Normal)}
		};
		return VertexInputAttributes;
	}	
};

struct Uniform 
{
	Matrix4X4 ViewProjMarix;

	static VkDescriptorSetLayoutBinding& UniformBinding()
	{
		static VkDescriptorSetLayoutBinding UniformSetLayoutBinding = 
		{
			0,
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			1,
			VK_SHADER_STAGE_VERTEX_BIT,
			nullptr
		};
		return UniformSetLayoutBinding;
	}

};

struct PushConstant
{
	Vector4 Color;
	Matrix4X4 WorldMatrix;

	static VkPushConstantRange& ConstansBinding()
	{
		static VkPushConstantRange PushConstantBinding = 
		{
			VK_SHADER_STAGE_VERTEX_BIT,
			0,
			sizeof(PushConstant)
		};
		return PushConstantBinding;
	}
};

struct GeometryInfo
{
	std::vector<Vertex> Vertices;
	std::vector<uint32_t> Indices;

	static GeometryInfo GetTriangleData()
	{
		static GeometryInfo Triangle = 
		{
		    /*{	
				Vertex({Vector3(-0.5, -0.5, 0), Vector3(1, 0, 0)}),
				Vertex({Vector3(0.5, -0.5, 0), Vector3(0, 0, 1)}),
				Vertex({Vector3(0, 0.5, 0), Vector3(0, 1, 0)})
			},*/
			{	
				Vertex({Vector3(0, -1, 0), Vector3(-1, 0, 0)}),
				Vertex({Vector3(0, 0, 1), Vector3(-1, 0, 0)}),
				Vertex({Vector3(0, 1, 0), Vector3(-1, 0, 0)})
			},
			{
				0, 1, 2 
			}
		};

		return Triangle;
	}

	static GeometryInfo GetBoxData()
	{
		static GeometryInfo Box = 
		{
			/*{
				Vertex({Vector3(-1, -1, -1), Vector3(1, 1, 1)}),
				Vertex({Vector3(-1, 1, -1), Vector3(0, 0, 0)}),
				Vertex({Vector3(1, 1, -1), Vector3(1, 0, 0)}),
				Vertex({Vector3(1, -1, -1), Vector3(0, 1, 0)}),
				Vertex({Vector3(-1, -1, 1), Vector3(0, 0, 1)}),
				Vertex({Vector3(-1, 1, 1), Vector3(1, 1, 0)}),
				Vertex({Vector3(1, 1, 1), Vector3(0, 1, 1)}),
				Vertex({Vector3(1, -1, 1), Vector3(1, 0, 1)})
			},*/
			{
				Vertex({Vector3(-0.5, -0.5, -0.5), Vector3(-0.5, -0.5, -0.5)}),
				Vertex({Vector3(-0.5, 0.5, -0.5), Vector3(-0.5, 0.5, -0.5)}),
				Vertex({Vector3(0.5, 0.5, -0.5), Vector3(0.5, 0.5, -0.5)}),
				Vertex({Vector3(0.5, -0.5, -0.5), Vector3(0.5, -0.5, -0.5)}),
				Vertex({Vector3(-0.5, -0.5, 0.5), Vector3(-0.5, -0.5, 0.5)}),
				Vertex({Vector3(-0.5, 0.5, 0.5), Vector3(-0.5, 0.5, 0.5)}),
				Vertex({Vector3(0.5, 0.5, 0.5), Vector3(0.5, 0.5, 0.5)}),
				Vertex({Vector3(0.5, -0.5, 0.5), Vector3(0.5, -0.5, 0.5)})
			},
			{
				0, 1, 2, 0, 2, 3,
				4, 6, 5, 4, 7, 6,
				4, 5, 1, 4, 1, 0,
				3, 2, 6, 3, 6, 7,
				1, 5, 6, 1, 6, 2,
				4, 0, 3, 4, 3, 7
			}
		};

		return Box;
	}
};

namespace VulkanUtil
{
	static void PrintVulkanInstanceSupport()
	{
		// 获取Vulkan实例支持扩展
		uint32_t VulkanInstExtensionsNum;
		CheckVR(vkEnumerateInstanceExtensionProperties(nullptr, &VulkanInstExtensionsNum, nullptr));
		VkExtensionProperties VulkanExtensionProperties[VulkanInstExtensionsNum];	
		CheckVR(vkEnumerateInstanceExtensionProperties(nullptr, &VulkanInstExtensionsNum, VulkanExtensionProperties));
		
		std::cout << "Vulkan实例支持扩展名:" << std::endl; 
		for(int i = 0; i < VulkanInstExtensionsNum; i++)
		{
			std::cout << "  " << VulkanExtensionProperties[i].extensionName << std::endl;
		}
	
		// 获取Vulkan实例可开启层
		uint32_t VulkanLayersNum;
		CheckVR(vkEnumerateInstanceLayerProperties(&VulkanLayersNum, nullptr));
		VkLayerProperties VulkanLayerProperties[VulkanLayersNum];
		CheckVR(vkEnumerateInstanceLayerProperties(&VulkanLayersNum, VulkanLayerProperties));
		
		std::cout << "Vulkan实例可开启层名:" << std::endl;
		for(int i = 0; i < VulkanLayersNum; i++)
		{
			std::cout << "  " << VulkanLayerProperties[i].layerName << std::endl;
		}
		
		std::cout << "---------------------------------------" << std::endl;
	}

	static void PrintPhysicalDeviceSupport(VkPhysicalDevice PhysicalDevice)
	{
		// 获取物理设备支持扩展
		uint32_t DeviceExtensionsNum;
		CheckVR(vkEnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &DeviceExtensionsNum, nullptr));
		VkExtensionProperties DeviceExtensionProperties[DeviceExtensionsNum];
		CheckVR(vkEnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &DeviceExtensionsNum, DeviceExtensionProperties));
	
		std::cout << "当前选择物理设备支持扩展名:" << std::endl;
		for(int i = 0; i < DeviceExtensionsNum; i++)
		{
			std::cout << "  " << DeviceExtensionProperties[i].extensionName << std::endl;
		}
	
		// 获取物理设备可开启层
		uint32_t DeviceLayersNum;
		CheckVR(vkEnumerateDeviceLayerProperties(PhysicalDevice, &DeviceLayersNum, nullptr));
		VkLayerProperties DeviceLayerProperties[DeviceLayersNum];
		CheckVR(vkEnumerateDeviceLayerProperties(PhysicalDevice, &DeviceLayersNum, DeviceLayerProperties));
	
		std::cout << "当前物理设备可开启层名:" << std::endl;
		for(int i = 0; i < DeviceLayersNum; i++)
		{
			std::cout << "  " << DeviceLayerProperties[i].layerName << std::endl;
		}	
		std::cout << "---------------------------------------" << std::endl;
	}

	static void PrintPhysicalDeviceInfo(VkPhysicalDevice PhysicalDevice)
	{
		// 查看物理设备属性
		VkPhysicalDeviceProperties PDProperties;
		vkGetPhysicalDeviceProperties(PhysicalDevice, &PDProperties);
		std::cout << "物理设备: " << PDProperties.deviceName << std::endl;
	
		VkPhysicalDeviceMemoryProperties PDMProperties;
		vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, &PDMProperties);
		std::cout << "内存堆数量:" << PDMProperties.memoryHeapCount << " ";
		std::cout << "内存类型数量:" << PDMProperties.memoryTypeCount << " ";
	
		std::cout << "内存类型标志:";
		for(int i = 0; i < PDMProperties.memoryTypeCount; i++)
		{
			std::cout << PDMProperties.memoryTypes[i].propertyFlags << " ";
		}
		std::cout << std::endl;
	
		uint32_t PDQueueFamilyPropertiesNum;
		vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &PDQueueFamilyPropertiesNum, nullptr);
		VkQueueFamilyProperties PDQueueFamilyProperties[PDQueueFamilyPropertiesNum];
		vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &PDQueueFamilyPropertiesNum, PDQueueFamilyProperties);
	
		std::cout << "队列族数量:" << PDQueueFamilyPropertiesNum << std::endl;
	
		std::cout << "支持图形操作的队列族索引为:";
		for(int i = 0; i < PDQueueFamilyPropertiesNum; i++)
		{
			if(PDQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				std::cout << i << " ";
			}
		}
		std::cout << std::endl;
	
		std::cout << "支持计算操作的队列族索引为:";
		for(int i = 0; i < PDQueueFamilyPropertiesNum; i++)
		{
			if(PDQueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				std::cout << i << " ";
			}
		}
		std::cout << std::endl;
		
		std::cout << "支持传输操作的队列族索引为:";
		for(int i = 0; i < PDQueueFamilyPropertiesNum; i++)
		{
			if(PDQueueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				std::cout << i << " ";
			}
		}
		std::cout << std::endl;
		std::cout << "---------------------------------------" << std::endl;
	}

	static uint32_t QueryQueueFamilyIndex(VkPhysicalDevice PhysicalDevice, VkQueueFlags QueueFlags)
	{
		uint32_t PDQueueFamilyPropertiesNum;
		vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &PDQueueFamilyPropertiesNum, nullptr);
		VkQueueFamilyProperties PDQueueFamilyProperties[PDQueueFamilyPropertiesNum];
		vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &PDQueueFamilyPropertiesNum, PDQueueFamilyProperties);
	
		for(uint32_t i = 0; i < PDQueueFamilyPropertiesNum; i++)	
		{
			if(PDQueueFamilyProperties[i].queueFlags & QueueFlags)
			{
				return i;
			}
		}
		throw "Not Find Physical Device's Queue Family";
	}

	static uint32_t QueryMemoryTypeIndex(VkPhysicalDevice PhysicalDevice, uint32_t TypeFilter, VkMemoryPropertyFlags Flag)
	{
		VkPhysicalDeviceMemoryProperties PDMProperties;
		vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, &PDMProperties);
		
		for(uint32_t i = 0; i < PDMProperties.memoryTypeCount; i++)
		{
			if(TypeFilter & (1 << i) && PDMProperties.memoryTypes[i].propertyFlags & Flag)
			{
				return i;
			}
		}
	
		throw "Not Find Physical Device's Memory";
	}

	static VkFormat QuerySupportedFormat(VkPhysicalDevice PhysicalDevice, uint32_t FormatsNum, VkFormat* Formats, VkImageTiling ImageTiling, VkFormatFeatureFlags FormatFeatures) 
	{
    	VkFormatProperties FormatProperties;
   		for(uint32_t i = 0; i < FormatsNum; i++)
   		{
       		vkGetPhysicalDeviceFormatProperties(PhysicalDevice, Formats[i], &FormatProperties);
        	if (ImageTiling == VK_IMAGE_TILING_LINEAR && FormatProperties.linearTilingFeatures & FormatFeatures) 
			{
        	    return Formats[i];
        	}
		   	if (ImageTiling == VK_IMAGE_TILING_OPTIMAL && FormatProperties.optimalTilingFeatures & FormatFeatures) 
			{
        	    return Formats[i];
        	}
    }
    throw "Not Find Supported Format";
}

	static VkResult CreateBuffer(VkPhysicalDevice PhysicalDevice, VkDevice LogicDevice, VkDeviceSize Size, VkBufferUsageFlags BufferUsage, VkMemoryPropertyFlags MemoryProperty, VkBuffer* pBuffer, VkDeviceMemory* pMemory)
	{
		VkBufferCreateInfo BufferInfo = { };
		BufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		BufferInfo.size = Size; 
		BufferInfo.usage = BufferUsage;
		BufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkResult VR = vkCreateBuffer(LogicDevice, &BufferInfo, nullptr, pBuffer);
		if(VR != VK_SUCCESS)
			return VR;

		VkMemoryRequirements MemoryRequirements;
		vkGetBufferMemoryRequirements(LogicDevice, *pBuffer, &MemoryRequirements);

		VkMemoryAllocateInfo MemoryInfo = { };
		MemoryInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		MemoryInfo.allocationSize = MemoryRequirements.size;
		MemoryInfo.memoryTypeIndex = QueryMemoryTypeIndex(PhysicalDevice, MemoryRequirements.memoryTypeBits, MemoryProperty);

		VR = vkAllocateMemory(LogicDevice, &MemoryInfo, nullptr, pMemory);
		if(VR != VK_SUCCESS)
			return VR;

		VR = vkBindBufferMemory(LogicDevice, *pBuffer, *pMemory, 0);
		if(VR != VK_SUCCESS)
			return VR;
		return VK_SUCCESS;
	}

	static VkResult CreateImage(VkPhysicalDevice PhysicalDevice, VkDevice LogicDevice,uint32_t Width, uint32_t Height, VkFormat Format, VkImageTiling ImageTiling, VkImageUsageFlags ImageUsage, VkMemoryPropertyFlags MemoryProperty, VkImage* pImage, VkDeviceMemory* pMemory) 
	{
		VkImageCreateInfo ImageInfo = { };
		ImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		ImageInfo.imageType = VK_IMAGE_TYPE_2D;
		ImageInfo.extent.width = Width;
		ImageInfo.extent.height = Height;
		ImageInfo.extent.depth = 1;
		ImageInfo.mipLevels = 1;
		ImageInfo.arrayLayers = 1;
		ImageInfo.format = Format;
		ImageInfo.tiling = ImageTiling;
		ImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		ImageInfo.usage = ImageUsage;
		ImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		ImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkResult VR = vkCreateImage(LogicDevice, &ImageInfo, nullptr, pImage);
		if(VR != VK_SUCCESS)
			return VR;

		VkMemoryRequirements MemoryRequirements;
		vkGetImageMemoryRequirements(LogicDevice, *pImage, &MemoryRequirements);

		VkMemoryAllocateInfo MemoryInfo = { };
		MemoryInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		MemoryInfo.allocationSize = MemoryRequirements.size;
		MemoryInfo.memoryTypeIndex = QueryMemoryTypeIndex(PhysicalDevice, MemoryRequirements.memoryTypeBits, MemoryProperty);

		VR = vkAllocateMemory(LogicDevice, &MemoryInfo, nullptr, pMemory);
		if(VR != VK_SUCCESS)
			return VR;

		VR = vkBindImageMemory(LogicDevice, *pImage, *pMemory, 0);
		if(VR != VK_SUCCESS)
			return VR;

		return VK_SUCCESS;
	}
}

