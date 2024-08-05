#include "RenderContext.h"
#include "GeometryManager.h"
#include "RenderUtil.h"
#include "CommandContext.h"
#include "ShaderManager.h"
#include "PipelineManager.h"

RenderContext::RenderContext()
{
}

RenderContext::~RenderContext()
{
}

void RenderContext::Init()
{
	CreateInstance();
	SelectPhysicalDevice();
	QueryPhysicalDeviceQueueIndex();
	CreateDevice();

	// Create Render
	Command = std::make_unique<CommandContext>();
	Shader = std::make_unique<ShaderManager>();
	Geometry = std::make_unique<GeometryManager>();
	Pipeline = std::make_unique<PipelineManager>();
	
	// Create Shader
	std::string ShaderPath = SHADER_PATH;
	CheckVR(Shader->CreateShader(ShaderType::VertexShader, "Box", ShaderPath + "Box.vert.spv"));
	CheckVR(Shader->CreateShader(ShaderType::FragmentShader, "Box", ShaderPath + "Box.frag.spv"));
	
	// Create Geometry
	GeometryInfo Box = GeometryInfo::GetBoxData();		
	CheckVR(Geometry->CreateGeometry("Box", Box.Vertices.size(), Box.Vertices.data(), Box.Indices.size(), Box.Indices.data()));
	GeometryInfo Triangle = GeometryInfo::GetTriangleData();;
	CheckVR(Geometry->CreateGeometry("Triangle", Triangle.Vertices.size(), Triangle.Vertices.data(), Triangle.Indices.size(), Triangle.Indices.data()));

	// Create Pipeline
	VkCustomPipelineCreateInfo xxdPipeline = {GraphicsPipelineType::Opaque, "Box", "Box"};
	CheckVR(Pipeline->CreateGraphicsPipeline("BoxPipeline", &xxdPipeline));
}

void RenderContext::CreateInstance()
{
	//VulkanUtil::PrintVulkanInstanceSupport();

	std::vector<const char*> InstanceExtensionNames;
	std::vector<const char*> InstanceLayerNames;

	InstanceExtensionNames.push_back("VK_KHR_get_physical_device_properties2");
	InstanceExtensionNames.push_back("VK_KHR_surface");
#if defined (__APPLE__)
	InstanceExtensionNames.push_back("VK_KHR_portability_enumeration"); 
	InstanceExtensionNames.push_back("VK_MVK_macos_surface");
    InstanceExtensionNames.push_back("VK_EXT_metal_surface");
#endif
	InstanceLayerNames.push_back("VK_LAYER_KHRONOS_validation");

	// 创建Vulkan实例
	VkApplicationInfo AppInfo = { };
	AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	AppInfo.apiVersion = VK_API_VERSION_1_2;
	AppInfo.pApplicationName = "VulkanDemo";
	AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.pEngineName = "VulkanDemo";
	AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	
	VkInstanceCreateInfo InstCreateInfo = { };
	InstCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;	
	InstCreateInfo.pApplicationInfo = &AppInfo;
	InstCreateInfo.enabledExtensionCount = InstanceExtensionNames.size();
	InstCreateInfo.ppEnabledExtensionNames = InstanceExtensionNames.data();
	InstCreateInfo.enabledLayerCount = InstanceLayerNames.size();
	InstCreateInfo.ppEnabledLayerNames = InstanceLayerNames.data();
	InstCreateInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

	CheckVR(vkCreateInstance(&InstCreateInfo, nullptr, &Instance));		
	
#if defined (__APPLE__)
	RenderFormat.ImageFormat = VK_FORMAT_B8G8R8A8_UNORM;
	RenderFormat.ImageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	RenderFormat.DepthStencilFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;
#endif
}

void RenderContext::SelectPhysicalDevice()
{
	// 获取物理显示设备
	uint32_t EnumPhysicalDevicesNum;
	CheckVR(vkEnumeratePhysicalDevices(Instance, &EnumPhysicalDevicesNum, nullptr));
	std::vector<VkPhysicalDevice> EnumPhysicalDevices(EnumPhysicalDevicesNum);
	CheckVR(vkEnumeratePhysicalDevices(Instance, &EnumPhysicalDevicesNum, &EnumPhysicalDevices[0]));

	for(int i = 0; i < EnumPhysicalDevicesNum; i++)
	{
		VulkanUtil::PrintPhysicalDeviceInfo(EnumPhysicalDevices[i]);
	}

	// 选择物理设备
	PhysicalDevice = EnumPhysicalDevices[0];
}

void RenderContext::QueryPhysicalDeviceQueueIndex()
{
	QueueFamilyIndexContent.Graphics = VulkanUtil::QueryQueueFamilyIndex(PhysicalDevice, VK_QUEUE_GRAPHICS_BIT);
	QueueFamilyIndexContent.Transfer = VulkanUtil::QueryQueueFamilyIndex(PhysicalDevice, VK_QUEUE_TRANSFER_BIT);
	QueueFamilyIndexContent.Compute = VulkanUtil::QueryQueueFamilyIndex(PhysicalDevice, VK_QUEUE_COMPUTE_BIT);
}

void RenderContext::CreateDevice()
{
	//VulkanUtil::PrintPhysicalDeviceSupport(PhysicalDevice);
	std::vector<const char*> DeviceExtensionNames;
	// 创建逻辑设备
	DeviceExtensionNames.push_back("VK_KHR_portability_subset");
	DeviceExtensionNames.push_back("VK_KHR_swapchain");

	VkPhysicalDeviceFeatures PDFeatures;
	vkGetPhysicalDeviceFeatures(PhysicalDevice, &PDFeatures);
	
	VkPhysicalDeviceFeatures EnableFeatures = { };
	EnableFeatures.multiDrawIndirect = PDFeatures.multiDrawIndirect;
	EnableFeatures.tessellationShader = VK_FALSE;
	EnableFeatures.geometryShader = VK_FALSE;
	EnableFeatures.fillModeNonSolid = VK_TRUE;

	std::set<uint32_t> QueueFamilyIndexSet;
	QueueFamilyIndexSet.insert(QueueFamilyIndexContent.Graphics);
	QueueFamilyIndexSet.insert(QueueFamilyIndexContent.Compute);
	QueueFamilyIndexSet.insert(QueueFamilyIndexContent.Transfer);

	float LDQueuePriorities = 1.f;
	
	std::vector<VkDeviceQueueCreateInfo> LDQueueCreateInfos;
	for(auto& Index : QueueFamilyIndexSet)
	{
		VkDeviceQueueCreateInfo DeviceQueueCreateInfo; 
		DeviceQueueCreateInfo = { };
		DeviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		DeviceQueueCreateInfo.queueFamilyIndex = Index;
		DeviceQueueCreateInfo.queueCount = 1;
		DeviceQueueCreateInfo.pQueuePriorities = &LDQueuePriorities; 

		LDQueueCreateInfos.push_back(std::move(DeviceQueueCreateInfo));
	}
	
	VkDeviceCreateInfo LDCreateInfo = { };
	LDCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	LDCreateInfo.queueCreateInfoCount = LDQueueCreateInfos.size();
	LDCreateInfo.pQueueCreateInfos = LDQueueCreateInfos.data();
	LDCreateInfo.pEnabledFeatures = &EnableFeatures; 
	LDCreateInfo.enabledExtensionCount = DeviceExtensionNames.size();
	LDCreateInfo.ppEnabledExtensionNames = DeviceExtensionNames.data();

	CheckVR(vkCreateDevice(PhysicalDevice, &LDCreateInfo, nullptr, &LogicDevice));

}

void RenderContext::Quit()
{
	vkDeviceWaitIdle(LogicDevice);
	Command.reset();
	Pipeline.reset();
	Geometry.reset();
	Shader.reset();
	vkDestroyDevice(LogicDevice, nullptr);
	vkDestroyInstance(Instance, nullptr);
}

