#include "Application.h"
#include "CqdWindow.h"
#include <SDL.h>
#include <memory>
#include "../Math/MathLibrary.h"
#include "../Render/RenderContext.h"
#include "../Render/RenderView.h"
#include "../Render/RenderUtil.h"
#include "World.h"
#include "InputSystem.h"
#include "../Cauto/LifeGame2DWorld.h"
#include "../Cauto/NaSchWorld.h"

Application::Application()
{
}

Application::~Application()
{
}

void Application::Create()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	RenderContext::Get().Init();

	MainWindow = std::make_shared<CqdWindow>(1280, 720); 
	MainView = std::make_shared<RenderView>(MainWindow);
	MainWorld = std::make_shared<NaSchWorld>();
	MainInput = std::make_shared<InputSystem>();
	MainView->BindWorld(MainWorld);

	IsQuit = false;
}

void Application::Destroy()
{
	MainInput.reset();
	MainWorld.reset();
	MainWindow.reset();
	MainView.reset();

	RenderContext::Get().Quit();
	SDL_Quit();
}

void Application::Run()
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

void Application::Quit()
{
	IsQuit = true;
}

std::weak_ptr<World> Application::GetMainWorld()
{
	return Application::Get().MainWorld;
}

std::weak_ptr<InputSystem> Application::GetMainInput()
{
	return Application::Get().MainInput;
}

std::weak_ptr<CqdWindow> Application::GetMainWindow()
{
	return Application::Get().MainWindow;
}

std::weak_ptr<RenderView> Application::GetMainView()
{
	return Application::Get().MainView;
}
