#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <unistd.h>
#include "Timer.h"
#include "World.h"
#include "InputSystem.h"
#include "CqdWindow.h"
#include "../Math/MathLibrary.h"
#include "../Render/RenderContext.h"
#include "../Render/RenderView.h"
#include "../Render/RenderUtil.h"
#include "../Template/Singleton.h"
#include "../Cauto/LifeGame2DWorld.h"

class Application : public Singleton<Application>
{
public:
	friend class Singleton<Application>;

	void Create(uint32_t WindowWidth = 1280, uint32_t WindowHeight = 720)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		RenderContext::Get().Init();
	
		MainWindow = std::make_shared<CqdWindow>(WindowWidth, WindowHeight); 
		MainView = std::make_shared<RenderView>(MainWindow);
		MainInput = std::make_shared<InputSystem>();
	
		IsQuit = false;
	}

	void Destroy()
	{
		if(MainWorld)
			MainWorld->Destroy();

		MainInput.reset();
		MainWorld.reset();
		MainWindow.reset();
		MainView.reset();
	
		RenderContext::Get().Quit();
		SDL_Quit();
	}

	void Run()
	{
		if(!MainWindow.get() || !MainView.get())
			return;

		MainWindow->Show();

		CreateWorld<LifeGame2DWorld>();
	
		AppTimer.Reset();
	
		while(!IsQuit)
		{
			if(!MainWindow.get() || !MainView.get() || !MainWorld.get())
				break;
	
			if(!MainWindow->PollEvent())
				break;
	
			AppTimer.Tick();
			if(!AppTimer.IsPaused())
			{
				MainInput->Update(AppTimer.GetDeltaSeconds());
				MainWorld->Update(AppTimer.GetDeltaSeconds());
				MainView->Render();
			}
			else
			{
				sleep(100);
			}
		}

	}

	void Quit()
	{
		IsQuit = true;
	}

	template<class WorldT = World>
	void CreateWorld()
	{
		if(MainWorld)
			MainWorld->Destroy();

		MainWorld = std::make_shared<WorldT>();
		MainView->BindWorld(MainWorld);

		MainWorld->Create();
	}

	static std::weak_ptr<World> GetMainWorld() { return Application::Get().MainWorld; }
	static std::weak_ptr<InputSystem> GetMainInput() { return Application::Get().MainInput; }
	static std::weak_ptr<CqdWindow> GetMainWindow() { return Application::Get().MainWindow; }
	static std::weak_ptr<RenderView> GetMainView() { return Application::Get().MainView; }

private:
	Application() = default;
	~Application() = default;

	bool IsQuit;

	Timer AppTimer;
	
	std::shared_ptr<CqdWindow> MainWindow;
	std::shared_ptr<RenderView> MainView;
	std::shared_ptr<World> MainWorld;
	std::shared_ptr<InputSystem> MainInput;
};

#endif
