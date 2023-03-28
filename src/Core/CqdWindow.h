#pragma once
#include <SDL_video.h>
#include "../Render/RenderInterface.h"

class CqdWindow : public IRenderSurface
{
public:
	CqdWindow(uint32_t Width, uint32_t Height);
	CqdWindow(CqdWindow& Copy) = delete;
	CqdWindow& operator=(CqdWindow&Copy) = delete;
	~CqdWindow();

	void GetWindowSize(uint32_t* Width, uint32_t* Height);

	int PollEvent();
	void Show();
	void SetWindowSize(uint32_t Width, uint32_t Height);

	friend class Application;
protected:
	virtual VkResult CreateSurface(VkInstance Instance, VkSurfaceKHR* pSurfaceKHR, VkAllocationCallbacks* pAllocationCallbacks) override;

private:
	uint32_t WindowWidth;
	uint32_t WindowHeight;
	SDL_Window* Window = nullptr;
};
