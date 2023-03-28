#pragma once
#include "../Template/Singleton.h"
#include <memory>
#include <vulkan/vulkan_core.h>
#include <set>

struct VkCustomQueueFamilyIndexContent
{
	uint8_t Graphics = -1;
	uint8_t Compute = -1;
	uint8_t Transfer = -1;
};

struct VkCustomRenderFormat
{
	VkFormat ImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
	VkColorSpaceKHR ImageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	VkFormat DepthStencilFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;
};

class RenderView;
class CommandContext;
class ShaderManager;
class PipelineManager;
class GeometryManager;
class RenderContext final : public Singleton<RenderContext>
{
public:
	friend class Singleton<RenderContext>;
	friend class RenderView;
	friend class CommandContext;
	friend class ShaderManager;
	friend class PipelineManager;
	friend class GeometryManager;

	void Init();
	void Quit();
private:
	RenderContext();
	~RenderContext();

	VkInstance Instance = nullptr;
	VkPhysicalDevice PhysicalDevice = nullptr;
	VkDevice LogicDevice = nullptr;

	VkCustomRenderFormat RenderFormat;
	VkCustomQueueFamilyIndexContent QueueFamilyIndexContent;

	std::unique_ptr<CommandContext> Command;
	std::unique_ptr<ShaderManager> Shader;
	std::unique_ptr<GeometryManager> Geometry;
	std::unique_ptr<PipelineManager> Pipeline;

	void CreateInstance();
	void SelectPhysicalDevice();
	void QueryPhysicalDeviceQueueIndex();
	void CreateDevice();
};
