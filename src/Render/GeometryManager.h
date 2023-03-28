#pragma once
#include "RenderUtil.h"
#include <memory>
#include <unordered_map>

struct VkCustomGeometryData
{
	friend class GeometryManager;

	uint32_t VertexNum;
	uint32_t IndexNum;

	VkBuffer VertexBuffer;
	VkBuffer IndexBuffer;
private:
	VkDeviceMemory VertexMemory;
	VkDeviceMemory IndexMemory;
};

class GeometryManager final
{
public:
	GeometryManager();
	~GeometryManager();

	std::vector<std::string> GetAllGeometryNames() const;
	VkResult CreateGeometry(std::string GeometryName, uint32_t VertexNum, const Vertex* VertexData, uint32_t IndexNum, const uint32_t* IndexData);
	VkCustomGeometryData* GetGeometryData(std::string GeometryName);
private:
	void DestroyAllGeometry();	
	
	std::unordered_map<std::string, std::unique_ptr<VkCustomGeometryData>> GeometryMap;
};
