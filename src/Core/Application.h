#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include "Timer.h"
#include "World.h"
#include "InputSystem.h"
#include "CqdWindow.h"
#include "../Math/MathLibrary.h"
#include "../Render/RenderContext.h"
#include "../Render/RenderView.h"
#include "../Render/RenderUtil.h"
#include "../Template/Singleton.h"

class Application : public Singleton<Application>
{
public:
	friend class Singleton<Application>;

	template<class WorldT = World>
	void Create(uint32_t WindowWidth = 1280, uint32_t WindowHeight = 720)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		RenderContext::Get().Init();
	
		MainWindow = std::make_shared<CqdWindow>(WindowWidth, WindowHeight); 
		MainView = std::make_shared<RenderView>(MainWindow);
		MainWorld = std::make_shared<WorldT>();
		MainInput = std::make_shared<InputSystem>();
		MainView->BindWorld(MainWorld);
	
		IsQuit = false;
	}

	void Destroy()
	{
		MainInput.reset();
		MainWorld.reset();
		MainWindow.reset();
		MainView.reset();
	
		RenderContext::Get().Quit();
		SDL_Quit();
	}

	void Run()
	{
		if(!MainWindow.get() || !MainView.get() || !MainWorld.get())
			return;

		MainWindow->Show();
		MainWorld->Create();
	
		AppTimer.Start();
	
		while(!IsQuit)
		{
			if(!MainWindow.get() || !MainView.get() || !MainWorld.get())
				break;
	
			if(!MainWindow->PollEvent())
				break;
	
			AppTimer.Tick();
			MainInput->Update(AppTimer.GetDeltaSeconds());
			MainWorld->Update(AppTimer.GetDeltaSeconds());
			MainView->Render();
		}

		MainWorld->Destroy();
	}

	void Quit()
	{
		IsQuit = true;
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
