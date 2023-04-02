#ifndef RENDER_VIEW_H
#define RENDER_VIEW_H

#include <memory>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "RenderInterface.h"

struct VkCustomDisplayInfo
{
	uint32_t ImageCount = 0;
	VkExtent2D ImageExtent = {0, 0};	
	VkFormat ImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
	VkColorSpaceKHR ImageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	VkFormat DepthStencilFormat = VK_FORMAT_D24_UNORM_S8_UINT;
};

struct Uniform;
class ShaderResource;

class RenderView final : public IRenderDraw 
{
public:
	RenderView(std::weak_ptr<IRenderSurface> ISurface);
	RenderView(RenderView& Copy) = delete;
	RenderView& operator=(RenderView& Copy) = delete;
	~RenderView();
	VkCustomDisplayInfo GetDisplayInfo() const;
	void BindWorld(std::weak_ptr<IRenderWorld> IWorld);

	void Render();
private:
	void InitRenderView();
	void ResetRenderView();
	void DestroyRenderView();

	void QuerySurfaceProperties();
	VkResult CreateSwapchain();
	VkResult CreateRenderTarget();	
	VkResult CreateDepthStencil();
	VkResult CreateFenceAndSemaphore();
	VkResult CreateFrameBuffers();

	virtual void DrawRenderItem(const RenderItem* pRenderItem) override;

	VkCustomDisplayInfo DisplayInfo;
	VkSurfaceKHR SurfaceKHR = nullptr;
	VkSwapchainKHR SwapchainKHR = nullptr;

	// Window Ref
	std::weak_ptr<IRenderSurface> ISurfaceWeak;
	std::weak_ptr<IRenderWorld> IWorldWeak;

	// RenderView
	VkViewport Viewport = { };
	VkRect2D Scissor = { };

	// RenderTarget
	uint32_t CurrRenderImageIndex = 0;
	std::vector<VkImage> RenderTargetImages;
	std::vector<VkImageView> RenderTargetViews;

	// DepthStencil
	VkImage DepthStencilImage = nullptr;
	VkDeviceMemory DepthStencilMemory = nullptr;
	VkImageView DepthStencilView = nullptr;

	std::vector<VkFramebuffer> FrameBuffers;
	std::unique_ptr<ShaderResource> RenderResource;

	VkFence RenderFence = nullptr;

};

#endif
