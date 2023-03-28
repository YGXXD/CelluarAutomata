#include "CqdWindow.h"
#include <SDL.h>
#include <SDL_vulkan.h>
#include <iostream>

CqdWindow::CqdWindow(uint32_t Width, uint32_t Height)
{
	WindowWidth = Width;
	WindowHeight = Height;

	// 创建Window
	Window = SDL_CreateWindow("Vulkan SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_VULKAN);
	if(!Window)
		throw "Window Create Failed";

	// SDL_SetWindowFullscreen(Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	// 获取SDL对Vulkan实例的扩展
	uint32_t SDLVulkanExtensionsNum;
	SDL_Vulkan_GetInstanceExtensions(Window, &SDLVulkanExtensionsNum, nullptr);
	const char* SDLVulkanExtensionNames[SDLVulkanExtensionsNum];
	SDL_Vulkan_GetInstanceExtensions(Window, &SDLVulkanExtensionsNum, &SDLVulkanExtensionNames[0]);
		
	std::cout << "SDL需要开启Vulkan实例扩展名:" << std::endl; 
	for(int i = 0; i < SDLVulkanExtensionsNum; i++)
	{
		std::cout << "  " << SDLVulkanExtensionNames[i] << std::endl; 
	}
	std::cout << "---------------------------------------" << std::endl;
}

CqdWindow::~CqdWindow()
{
	SDL_DestroyWindow(Window);
	std::cout << "CqdWindow Is Destroyed" << std::endl;
}

void CqdWindow::GetWindowSize(uint32_t* Width, uint32_t* Height)
{
	*Width = WindowWidth;
	*Height = WindowHeight;
}

void CqdWindow::Show()
{
	SDL_ShowWindow(Window);
}

void CqdWindow::SetWindowSize(uint32_t Width, uint32_t Height)
{
	WindowWidth = Width;
	WindowHeight = Height;
	SDL_SetWindowSize(Window, Width, Height);
	ResetRenderView();
}

int CqdWindow::PollEvent()
{
	SDL_Event Event;
	while(SDL_PollEvent(&Event)) 
	{
		if(Event.type == SDL_QUIT)
		{
			return 0;
		}
	}
	return 1;
}

VkResult CqdWindow::CreateSurface(VkInstance Instance, VkSurfaceKHR* pSurfaceKHR, VkAllocationCallbacks* pAllocationCallbacks)
{
	// 创建VulkanSurface
	if(!SDL_Vulkan_CreateSurface(Window, Instance, pSurfaceKHR))
	{
		return VK_ERROR_UNKNOWN;
	}
	return VK_SUCCESS;
}
