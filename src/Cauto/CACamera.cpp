#include "CACamera.h"
#include "../Core/InputSystem.h"
#include "../Core/Application.h"
#include "LifeGame2DWorld.h"
#include "LifeGame3DWorld.h"
#include "Wolfram184World.h"
#include "NaSchWorld.h"
#include "BMLWorld.h"
#include "TrafficLightWorld.h"
#include "CrossRoadWorld.h"
#include "RoadInstWorld.h"

int CACamera::WorldIdx = 1;

void CACamera::EnterNextWorld()
{
	WorldIdx = (WorldIdx + 1) % 8;

	switch (WorldIdx)
	{
		case 0:
			Application::Get().CreateWorld<LifeGame2DWorld>();
			break;
		case 1:
			Application::Get().CreateWorld<LifeGame3DWorld>();
			break;
		case 2:
			Application::Get().CreateWorld<Wolfram184World>();
			break;
		case 3:
			Application::Get().CreateWorld<NaSchWorld>();
			break;
		case 4:
			Application::Get().CreateWorld<BMLWorld>();
			break;
		case 5:
			Application::Get().CreateWorld<CrossRoadWorld>();
			break;
		case 6:
			Application::Get().CreateWorld<TrafficLightWorld>();
			break;
		case 7:
			Application::Get().CreateWorld<RoadInstWorld>();
			break;
		default:
			return;
	}

}

void CACamera::SetUpInput(InputSystem* MainInput)
{
	Camera::SetUpInput(MainInput);
	MainInput->ActionBind(SDL_SCANCODE_DOWN, InputType::OnPressed, this, &CACamera::EnterNextWorld);
}
