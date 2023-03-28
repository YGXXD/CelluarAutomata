#pragma once
#include <memory>
#include "Timer.h"
#include "../Template/Singleton.h"
#include "../Math/Matrix4X4.h"
#include "../Math/Vector3.h"

class CqdWindow;
class RenderView;
class World;
class InputSystem;
struct Uniform;
class Application : public Singleton<Application>
{
public:
	friend class Singleton<Application>;

	void Create();
	void Destroy();
	void Run();
	void Quit();

	static std::weak_ptr<World> GetMainWorld();
	static std::weak_ptr<InputSystem> GetMainInput();
	static std::weak_ptr<CqdWindow> GetMainWindow();
	static std::weak_ptr<RenderView> GetMainView();

private:
	Application();
	~Application();

	bool IsQuit;

	Timer AppTimer;
	
	std::shared_ptr<CqdWindow> MainWindow;
	std::shared_ptr<RenderView> MainView;
	std::shared_ptr<World> MainWorld;
	std::shared_ptr<InputSystem> MainInput;
};

