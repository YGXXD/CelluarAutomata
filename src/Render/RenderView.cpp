#include "RenderView.h"
#include "CommandContext.h"
#include "GeometryManager.h"
#include "PipelineManager.h"
#include "ShaderManager.h"
#include "RenderContext.h"
#include "RenderUtil.h"
#include "RenderInterface.h"
#include "../Math/MathLibrary.h"
#include <iostream>

RenderView::RenderView(std::weak_ptr<IRenderSurface> ISurface)
{
	if(ISurface.expired())
		return;

	ISurface.lock()->ResetRenderView.Bind(this, &RenderView::ResetRenderView);
	CheckVR(ISurface.lock()->CreateSurface(RenderContext::Get().Instance, &SurfaceKHR, nullptr));

	ISurfaceWeak = ISurface;
	InitRenderView();
}

RenderView::~RenderView()
{
	if(!ISurfaceWeak.expired())
		ISurfaceWeak.lock()->ResetRenderView.Clear();
	DestroyRenderView();

	std::cout << "RenderView Is Destroyed" << std::endl;
}

VkCustomDisplayInfo RenderView::GetDisplayInfo() const
{
	return DisplayInfo;
}

void RenderView::BindWorld(std::weak_ptr<IRenderWorld> IWorld)
{
	IWorldWeak = IWorld;
}

void RenderView::Render()
{
	if(IWorldWeak.expired())
	{
		return;
	}

	if(!RenderResource.get())
	{
		return;
	}
	
	CommandContext* Cmd = RenderContext::Get().Command.get();
	PipelineManager* Pmg = RenderContext::Get().Pipeline.get();
	if(!Cmd || !Pmg)
	{
		return;
	}

	VkDevice LogicDevice = RenderContext::Get().LogicDevice;

	// 等待下一帧Image
	VkResult VR = vkAcquireNextImageKHR(LogicDevice, SwapchainKHR, 0xffffffff, nullptr, RenderFence, &CurrRenderImageIndex);
	if(VR != VK_SUCCESS)
	{
		std::cout << "Acquire Next Image Faild!" << std::endl;
		return;
	}
    VR = vkWaitForFences(LogicDevice, 1, &RenderFence, VK_TRUE, 0xffffffff);
	if(VR != VK_SUCCESS)
	{
		std::cout << "Wait Fence Faild!" << std::endl;
		return;
	}
	vkResetFences(LogicDevice, 1, &RenderFence);

	// 计算投影矩阵
	CameraDataPak DataPack = { };
	IWorldWeak.lock()->UpdateCameraDataPak(&DataPack);
	Vector3 Up = Vector3::Up();
    Matrix4X4 CameraView = Math::MatrixLookAt(DataPack.WorldLocation, DataPack.Focus, Up);
	Matrix4X4 CameraProj = Math::MatrixPerspectiveFov(DataPack.FOV, (float)DisplayInfo.ImageExtent.width / (float)DisplayInfo.ImageExtent.height, DataPack.NearZ, DataPack.FarZ);
	CameraProj.m22 = -CameraProj.m22;	

	// 上传Uniform
	Uniform UniformData = { CameraView * CameraProj };
	RenderResource->UploadUniformBuffer(&UniformData);

	// 渲染
	Cmd->GraphicsCmdBegin();
	Cmd->GraphicsCmdBeginRender(CurrRenderImageIndex, Pmg->GetRenderPass(), FrameBuffers[CurrRenderImageIndex], &Scissor);
	Cmd->GraphicsCmdBindDiscriptor(Pmg->GetPileineLayout(), RenderResource->GetDescriptorSet());
	Cmd->GraphicsCmdBindViewport(&Viewport);
	Cmd->GraphicsCmdBindScissor(&Scissor);

	IWorldWeak.lock()->Draw(this);

	Cmd->GraphicsCmdEndRender();
	Cmd->GraphicsCmdEnd();
	Cmd->SubmitGraphicsCommand(RenderFence);

	// 呈现
	Cmd->GraphicsQueuePresent(&SwapchainKHR, &CurrRenderImageIndex);

    VR = vkWaitForFences(LogicDevice, 1, &RenderFence, VK_TRUE, 0xffffffff);
	if(VR != VK_SUCCESS)
	{
		std::cout << "Wait Fence Faild!" << std::endl;
		return;
	}
	vkResetFences(LogicDevice, 1, &RenderFence);

	Cmd->ResetGraphicsCommand();
}

void RenderView::InitRenderView()
{
	QuerySurfaceProperties();
	CheckVR(CreateSwapchain());
	CheckVR(CreateRenderTarget());
	CheckVR(CreateDepthStencil());
	CheckVR(CreateFenceAndSemaphore());
	CheckVR(CreateFrameBuffers());

	RenderResource = std::make_unique<ShaderResource>();
	RenderResource->CreateShaderResource(RenderContext::Get().PhysicalDevice, RenderContext::Get().LogicDevice);
}

void RenderView::ResetRenderView()
{ 
	// 重新查询视口参数
	QuerySurfaceProperties();

	// 重新创建交换链
	vkDestroySwapchainKHR(RenderContext::Get().LogicDevice, SwapchainKHR, nullptr);
	SwapchainKHR = nullptr;
	CreateSwapchain();
	
	// 重新创建渲染图
	for(auto& ImageView : RenderTargetViews)
		vkDestroyImageView(RenderContext::Get().LogicDevice, ImageView, nullptr);
	RenderTargetViews.clear();
	CreateRenderTarget();

	// 重新创建深度图
	vkDestroyImageView(RenderContext::Get().LogicDevice, DepthStencilView, nullptr);
	DepthStencilView = nullptr;
	vkFreeMemory(RenderContext::Get().LogicDevice, DepthStencilMemory, nullptr);
	DepthStencilMemory = nullptr;
	vkDestroyImage(RenderContext::Get().LogicDevice, DepthStencilImage, nullptr);
	DepthStencilImage = nullptr;
	CreateDepthStencil();

	// 重新创建帧缓冲
	for(auto& FrameBuffer : FrameBuffers)
	{
		vkDestroyFramebuffer(RenderContext::Get().LogicDevice, FrameBuffer, nullptr);
	}
	FrameBuffers.clear();
	CreateFrameBuffers();
}

void RenderView::DestroyRenderView()
{
	vkDestroyImageView(RenderContext::Get().LogicDevice, DepthStencilView, nullptr);
	vkFreeMemory(RenderContext::Get().LogicDevice, DepthStencilMemory, nullptr);
	vkDestroyImage(RenderContext::Get().LogicDevice, DepthStencilImage, nullptr);

	for(auto& ImageView : RenderTargetViews)
		vkDestroyImageView(RenderContext::Get().LogicDevice, ImageView, nullptr);
	RenderTargetViews.clear();

	for(auto& FrameBuffer : FrameBuffers)
	{
		vkDestroyFramebuffer(RenderContext::Get().LogicDevice, FrameBuffer, nullptr);
	}
	FrameBuffers.clear();

	if(RenderResource.get())
		RenderResource->DestroyShaderResource(RenderContext::Get().LogicDevice);

	vkDestroySwapchainKHR(RenderContext::Get().LogicDevice, SwapchainKHR, nullptr);
	vkDestroyFence(RenderContext::Get().LogicDevice, RenderFence, nullptr);
	vkDestroySurfaceKHR(RenderContext::Get().Instance, SurfaceKHR, nullptr);
}

void RenderView::QuerySurfaceProperties()
{
	// 获取交换链对于Surface支持的参数
	VkSurfaceCapabilitiesKHR SurfaceCapabilitiesKHR;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(RenderContext::Get().PhysicalDevice, SurfaceKHR,  &SurfaceCapabilitiesKHR);

	std::cout << "当前Surface支持的参数:" << std::endl;
	std::cout << "  " << "minImageCount:" << SurfaceCapabilitiesKHR.minImageCount << std::endl;
	std::cout << "  " << "maxImageCount:" << SurfaceCapabilitiesKHR.maxImageCount << std::endl;
	std::cout << "  " << "currentExtent:" << SurfaceCapabilitiesKHR.currentExtent.width << "," << SurfaceCapabilitiesKHR.currentExtent.height << std::endl;
	std::cout << "  " << "minImageExtent:" << SurfaceCapabilitiesKHR.minImageExtent.width << "," << SurfaceCapabilitiesKHR.minImageExtent.height << std::endl;
	std::cout << "  " << "maxImageExtent:" << SurfaceCapabilitiesKHR.maxImageExtent.width << "," << SurfaceCapabilitiesKHR.maxImageExtent.height << std::endl;
	std::cout << "  " << "maxImageArrayLayers:" << SurfaceCapabilitiesKHR.maxImageArrayLayers << std::endl;

	uint32_t SupportPresentModesNum;
	vkGetPhysicalDeviceSurfacePresentModesKHR(RenderContext::Get().PhysicalDevice, SurfaceKHR, &SupportPresentModesNum, nullptr);
	std::vector<VkPresentModeKHR> SupportPresentModes(SupportPresentModesNum);
	vkGetPhysicalDeviceSurfacePresentModesKHR(RenderContext::Get().PhysicalDevice, SurfaceKHR, &SupportPresentModesNum, &SupportPresentModes[0]);
	
	std::cout << "  " << "PresentModes:";
	for(int i = 0; i < SupportPresentModesNum; i++)
	{
		std::cout << SupportPresentModes[i] << " ";
	}
	std::cout << std::endl;
	
	DisplayInfo.ImageFormat = RenderContext::Get().RenderFormat.ImageFormat;
	DisplayInfo.ImageColorSpace = RenderContext::Get().RenderFormat.ImageColorSpace;
	DisplayInfo.ImageCount = 2;
	DisplayInfo.ImageExtent.width = Math::Clamp(SurfaceCapabilitiesKHR.minImageExtent.width, SurfaceCapabilitiesKHR.maxImageExtent.width, SurfaceCapabilitiesKHR.currentExtent.width);
	DisplayInfo.ImageExtent.height = Math::Clamp(SurfaceCapabilitiesKHR.minImageExtent.height, SurfaceCapabilitiesKHR.maxImageExtent.height, SurfaceCapabilitiesKHR.currentExtent.height);
	DisplayInfo.DepthStencilFormat = RenderContext::Get().RenderFormat.DepthStencilFormat;

	Viewport = {0, 0, (float)DisplayInfo.ImageExtent.width, (float)DisplayInfo.ImageExtent.height, 0, 1};
	Scissor = {{0, 0}, DisplayInfo.ImageExtent};

	uint32_t SupportFormatsNum;
	vkGetPhysicalDeviceSurfaceFormatsKHR(RenderContext::Get().PhysicalDevice, SurfaceKHR, &SupportFormatsNum, nullptr);
	std::vector<VkSurfaceFormatKHR> SupportFormatsKHR(SupportFormatsNum);
	vkGetPhysicalDeviceSurfaceFormatsKHR(RenderContext::Get().PhysicalDevice, SurfaceKHR, &SupportFormatsNum, &SupportFormatsKHR[0]);

	bool IsSupportRenderFormat = false;
	std::cout << "  " << "SurfaceFormats And ColorSpace:";
	for(int i = 0; i < SupportFormatsNum; i++)
	{
		std::cout << SupportFormatsKHR[i].format << ",";
		std::cout << SupportFormatsKHR[i].colorSpace << " ";
		if(DisplayInfo.ImageFormat == SupportFormatsKHR[i].format && DisplayInfo.ImageColorSpace == SupportFormatsKHR[i].colorSpace)
			IsSupportRenderFormat = true;
	}
	std::cout << std::endl;
	std::cout << "---------------------------------------" << std::endl;
	
	VulkanUtil::QuerySupportedFormat(RenderContext::Get().PhysicalDevice, 1, &DisplayInfo.DepthStencilFormat, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

	if(!IsSupportRenderFormat)
		throw "Surface Not Support Curr RenderFormat";
}

VkResult RenderView::CreateSwapchain()
{
	// 创建交换链
	VkSwapchainCreateInfoKHR SwapChainInfo = { };
	SwapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	SwapChainInfo.surface = SurfaceKHR;
	SwapChainInfo.minImageCount = DisplayInfo.ImageCount;
	SwapChainInfo.imageFormat = DisplayInfo.ImageFormat;
	SwapChainInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	SwapChainInfo.imageExtent = DisplayInfo.ImageExtent;
	SwapChainInfo.imageArrayLayers = 1;
	SwapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	SwapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	SwapChainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	SwapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	SwapChainInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
	SwapChainInfo.clipped = VK_TRUE;
	SwapChainInfo.oldSwapchain = SwapchainKHR;

	return vkCreateSwapchainKHR(RenderContext::Get().LogicDevice, &SwapChainInfo, nullptr, &SwapchainKHR);
}

VkResult RenderView::CreateRenderTarget()
{
	RenderTargetImages.resize(DisplayInfo.ImageCount);
	VkResult VR = vkGetSwapchainImagesKHR(RenderContext::Get().LogicDevice, SwapchainKHR, &DisplayInfo.ImageCount, RenderTargetImages.data());
	if(VR != VK_SUCCESS)
		return VR;

	RenderTargetViews.resize(DisplayInfo.ImageCount);

	for(int i = 0; i < RenderTargetViews.size(); i++)
	{
		VkImageViewCreateInfo ImageViewInfo = { };
		ImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		ImageViewInfo.image = RenderTargetImages[i];
		ImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		ImageViewInfo.format = DisplayInfo.ImageFormat;
		ImageViewInfo.components = {VK_COMPONENT_SWIZZLE_IDENTITY, 
									VK_COMPONENT_SWIZZLE_IDENTITY,
									VK_COMPONENT_SWIZZLE_IDENTITY,
									VK_COMPONENT_SWIZZLE_IDENTITY};
		ImageViewInfo.subresourceRange.baseMipLevel = 0;
		ImageViewInfo.subresourceRange.levelCount = 1;
		ImageViewInfo.subresourceRange.baseArrayLayer = 0;
		ImageViewInfo.subresourceRange.layerCount = 1;
		ImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		VR = vkCreateImageView(RenderContext::Get().LogicDevice, &ImageViewInfo, nullptr, &RenderTargetViews[i]);
		if(VR != VK_SUCCESS)
			return VR;
	}

	return VK_SUCCESS;	
}

VkResult RenderView::CreateDepthStencil()
{
	VkResult VR = VulkanUtil::CreateImage(RenderContext::Get().PhysicalDevice, RenderContext::Get().LogicDevice, DisplayInfo.ImageExtent.width, DisplayInfo.ImageExtent.height, DisplayInfo.DepthStencilFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &DepthStencilImage, &DepthStencilMemory);
	if(VR != VK_SUCCESS)
		return VR;

	VkImageViewCreateInfo ImageViewInfo = { };
	ImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	ImageViewInfo.image = DepthStencilImage;
	ImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	ImageViewInfo.format = DisplayInfo.DepthStencilFormat;
	ImageViewInfo.components = {VK_COMPONENT_SWIZZLE_IDENTITY, 
								VK_COMPONENT_SWIZZLE_IDENTITY,
								VK_COMPONENT_SWIZZLE_IDENTITY,
								VK_COMPONENT_SWIZZLE_IDENTITY};
	ImageViewInfo.subresourceRange.baseMipLevel = 0;
	ImageViewInfo.subresourceRange.levelCount = 1;
	ImageViewInfo.subresourceRange.baseArrayLayer = 0;
	ImageViewInfo.subresourceRange.layerCount = 1;
	ImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

	VR = vkCreateImageView(RenderContext::Get().LogicDevice, &ImageViewInfo, nullptr, &DepthStencilView);

	return VK_SUCCESS;
}

VkResult RenderView::CreateFenceAndSemaphore()
{
	VkFenceCreateInfo FenceInfo = { };
	FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	VkResult VR = vkCreateFence(RenderContext::Get().LogicDevice, &FenceInfo, nullptr, &RenderFence);
	if(VR != VK_SUCCESS)
		return VR;

	return VK_SUCCESS;
}

VkResult RenderView::CreateFrameBuffers()
{
	PipelineManager* Pipeline = RenderContext::Get().Pipeline.get();
	if(!Pipeline)
		return VK_ERROR_UNKNOWN;

	VkRenderPass PipelineRenderPass = Pipeline->GetRenderPass();
	if(!PipelineRenderPass)
		return VK_ERROR_UNKNOWN;
	
	VkResult VR = VK_ERROR_UNKNOWN;
	FrameBuffers.resize(DisplayInfo.ImageCount);
	for(int i = 0; i < DisplayInfo.ImageCount; i++)
	{
		VkImageView AttachImageView[2] = { RenderTargetViews[i], DepthStencilView };
		VkFramebufferCreateInfo FrameBufferInfo = { };
		FrameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		FrameBufferInfo.attachmentCount = 2;
		FrameBufferInfo.pAttachments = AttachImageView;
		FrameBufferInfo.renderPass = PipelineRenderPass; 
		FrameBufferInfo.width = DisplayInfo.ImageExtent.width;
		FrameBufferInfo.height = DisplayInfo.ImageExtent.height;
		FrameBufferInfo.layers = 1;
	
		VR = vkCreateFramebuffer(RenderContext::Get().LogicDevice, &FrameBufferInfo, nullptr, &(FrameBuffers[i]));
		if(VR != VK_SUCCESS)
			return VR;
	}
	return VK_SUCCESS;
}

void RenderView::DrawRenderItem(const RenderItem* pRenderItem)
{
	VkCustomGeometryData* GeometryData = RenderContext::Get().Geometry->GetGeometryData(pRenderItem->GeometryName);
	VkPipeline GraphicsPipeline = RenderContext::Get().Pipeline->GetGraphicsPipeline(pRenderItem->PipelineName);
	if(!GeometryData | !GraphicsPipeline)
		return;

	CommandContext* Cmd = RenderContext::Get().Command.get();
	PipelineManager* Pmg = RenderContext::Get().Pipeline.get();
	
	Vector3 RenderLocation = pRenderItem->WorldLocation;
	Vector3 RenderScale = pRenderItem->WorldScale;
	Matrix4X4 MT = Math::MatrixTransition(RenderLocation);
	Matrix4X4 MS = Math::MatrixScale(RenderScale);
	PushConstant Constants = { Vector4(pRenderItem->RenderColor, 1), MS * MT };
	
	Cmd->GraphicsCmdBindPipeline(GraphicsPipeline);
	Cmd->GraphicsCmdPushConstants(Pmg->GetPileineLayout(), &Constants);
	Cmd->GraphicsCmdBindVertices(GeometryData->VertexBuffer);
	Cmd->GraphicsCmdBindIndices(GeometryData->IndexBuffer);
	Cmd->GraphicsCmdDrawIndices(GeometryData->IndexNum, 1, 0);
}
