#include "GeometryManager.h"
#include "CommandContext.h"
#include "RenderContext.h"
#include "RenderUtil.h"
#include <cstring>

GeometryManager::GeometryManager()
{
}

GeometryManager::~GeometryManager()
{
	DestroyAllGeometry();
}

std::vector<std::string> GeometryManager::GetAllGeometryNames() const
{
	std::vector<std::string> Names;
	for(auto& GeometryInfo : GeometryMap)
	{
		Names.push_back(GeometryInfo.first);
	}
	return Names;
}

VkCustomGeometryData* GeometryManager::GetGeometryData(std::string GeometryName)
{
	if(GeometryMap.count(GeometryName))
		return GeometryMap[GeometryName].get();
	return nullptr;
}

VkResult GeometryManager::CreateGeometry(std::string GeometryName, uint32_t VertexNum, const Vertex* VertexData, uint32_t IndexNum, const uint32_t* IndexData)
{
	if(GeometryMap.count(GeometryName))
		return VK_ERROR_UNKNOWN;

	GeometryMap[GeometryName] = std::make_unique<VkCustomGeometryData>();
	VkCustomGeometryData* GeometryData = GeometryMap[GeometryName].get();

	// 顶点缓冲区
	VkBuffer VertexBufferSRC;
	VkDeviceMemory VertexMemorySRC;	

	GeometryData->VertexNum = VertexNum;
	VkDeviceSize VertexSize = VertexNum * sizeof(Vertex);

	VkResult VR = VulkanUtil::CreateBuffer(RenderContext::Get().PhysicalDevice, RenderContext::Get().LogicDevice, VertexSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &VertexBufferSRC, &VertexMemorySRC);
	if(VR != VK_SUCCESS)
		return VR;

	void* pVertexMapData;
	VR = vkMapMemory(RenderContext::Get().LogicDevice, VertexMemorySRC, 0, VertexSize, 0, &pVertexMapData);
	if(VR != VK_SUCCESS)
		return VR;

	memcpy(pVertexMapData, VertexData, VertexSize);
	vkUnmapMemory(RenderContext::Get().LogicDevice, VertexMemorySRC);

	VR = VulkanUtil::CreateBuffer(RenderContext::Get().PhysicalDevice, RenderContext::Get().LogicDevice, VertexSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &(GeometryMap[GeometryName]->VertexBuffer), &(GeometryMap[GeometryName]->VertexMemory));
	if(VR != VK_SUCCESS)
		return VR;

	// 索引缓冲区
	VkBuffer IndexBufferSRC;
	VkDeviceMemory IndexMemorySRC;	

	GeometryData->IndexNum = IndexNum;
	VkDeviceSize IndexSize = IndexNum * sizeof(uint32_t);

	VR = VulkanUtil::CreateBuffer(RenderContext::Get().PhysicalDevice, RenderContext::Get().LogicDevice, VertexSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &IndexBufferSRC, &IndexMemorySRC);
	if(VR != VK_SUCCESS)
		return VR;

	void* pIndexMapData;
	VR = vkMapMemory(RenderContext::Get().LogicDevice, IndexMemorySRC, 0, IndexSize, 0, &pIndexMapData);
	if(VR != VK_SUCCESS)
		return VR;
	memcpy(pIndexMapData, IndexData, IndexSize);
	vkUnmapMemory(RenderContext::Get().LogicDevice, IndexMemorySRC);

	VR = VulkanUtil::CreateBuffer(RenderContext::Get().PhysicalDevice, RenderContext::Get().LogicDevice, IndexSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &(GeometryMap[GeometryName]->IndexBuffer), &(GeometryMap[GeometryName]->IndexMemory));
	if(VR != VK_SUCCESS)
		return VR;

	// 传输操作
	CommandContext* Cmd = RenderContext::Get().Command.get();
	Cmd->TransferCmdBegin();
	Cmd->TransferCmdCopyBuffer(VertexBufferSRC, GeometryData->VertexBuffer, VertexSize);
	Cmd->TransferCmdCopyBuffer(IndexBufferSRC, GeometryData->IndexBuffer, IndexSize);
	Cmd->TransferCmdEnd();
	Cmd->SubmitTransferCommand(nullptr);
	vkDeviceWaitIdle(RenderContext::Get().LogicDevice);
	Cmd->ResetTransferCommand();

	vkFreeMemory(RenderContext::Get().LogicDevice, VertexMemorySRC, nullptr);
	vkFreeMemory(RenderContext::Get().LogicDevice, IndexMemorySRC, nullptr);
	vkDestroyBuffer(RenderContext::Get().LogicDevice, VertexBufferSRC, nullptr);
	vkDestroyBuffer(RenderContext::Get().LogicDevice, IndexBufferSRC, nullptr);

	return VK_SUCCESS;
}

void GeometryManager::DestroyAllGeometry()
{
	for(auto& GeometryData : GeometryMap)
	{
		vkFreeMemory(RenderContext::Get().LogicDevice, GeometryData.second->VertexMemory, nullptr);
		vkFreeMemory(RenderContext::Get().LogicDevice, GeometryData.second->IndexMemory, nullptr);
		vkDestroyBuffer(RenderContext::Get().LogicDevice, GeometryData.second->VertexBuffer, nullptr);
		vkDestroyBuffer(RenderContext::Get().LogicDevice, GeometryData.second->IndexBuffer, nullptr);
	}
}
