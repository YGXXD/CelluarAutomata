#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

#include <string>
#include <vulkan/vulkan_core.h>
#include "../Math/Vector3.h"
#include "../Template/Delegate.h"

struct RenderItem
{
	Vector3 WorldLocation;
	Vector3 WorldScale;
	Vector3 RenderColor;
	std::string GeometryName;
	std::string PipelineName;
};

class IRenderDraw
{
public:
	virtual void DrawRenderItem(const RenderItem* pRenderItem) = 0;
protected:
	virtual ~IRenderDraw() { };
};

class IRenderSurface
{
public:
	DECLARE_FUNCTION_DELEGATE_NO_PARAMETER(ResetRenderViewDelegate, void);
	virtual VkResult CreateSurface(VkInstance Insctance, VkSurfaceKHR* pSurface, VkAllocationCallbacks* pAllocation) = 0;

	ResetRenderViewDelegate ResetRenderView;
protected:
	virtual ~IRenderSurface() { };
};

struct CameraDataPak
{
	float FOV;
	float NearZ;
	float FarZ;
	Vector3 WorldLocation;
	Vector3 Focus;
};

class IRenderWorld
{
public:
	virtual void Draw(IRenderDraw* pDraw) = 0;
	virtual void UpdateCameraDataPak(CameraDataPak* pDataPak) = 0;
protected:
	virtual ~IRenderWorld() { };
};

#endif
