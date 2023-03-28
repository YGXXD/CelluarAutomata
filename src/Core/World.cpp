#include "World.h"
#include <iostream>
#include <memory>
#include "Application.h"
#include "Camera.h"
#include "InputSystem.h"

World::World()
{
}

World::~World()
{
	std::cout << "World Is Destroyed" << std::endl;
}

void World::Draw(IRenderDraw* pDraw)
{
	for(auto& ActorInfo : ActorMap)
	{
		std::shared_ptr<Actor> ActorShared = ActorInfo.second;
		if(ActorShared->IsHidden())
			continue;

		RenderItem Item;
		Item.GeometryName = ActorShared->GetGeometryName();
		Item.PipelineName = ActorShared->GetPipelineName();		
		Item.RenderColor = ActorShared->GetRenderColor();
		Item.WorldLocation = ActorShared->GetActorLocation();
		Item.WorldScale = ActorShared->GetActorScale();
		
		pDraw->DrawRenderItem(&Item);	
	}	
}

void World::UpdateCameraDataPak(CameraDataPak* pDataPak)
{
	if(!CameraWeak.expired())
	{
		auto CameraShared = CameraWeak.lock();
		pDataPak->FOV = CameraShared->GetFOV();
		pDataPak->NearZ = CameraShared->GetNearZ();
		pDataPak->FarZ = CameraShared->GetFarZ();
		pDataPak->Focus = CameraShared->GetFocus();
		pDataPak->WorldLocation = CameraShared->GetActorLocation();
	}
}

void World::Create()
{
	BeginCreate();
	CameraWeak = GetActorFromClass<Camera>();	
	if(CameraWeak.expired())
	{
		CameraWeak = SpawnActor<Camera>("Camera");
	}
	InputSystem* MainInput = Application::GetMainInput().lock().get();
	if(MainInput)
	{
		MainInput->PossessActor(CameraWeak);
	}
}

void World::BeginCreate()
{
}

void World::Update(float DeltaSeconds)
{
	WorldUpdate(DeltaSeconds);

	for(auto& ActorInfo : ActorMap)
	{
		if(ActorInfo.second->IsUpdate())
			ActorInfo.second->Update(DeltaSeconds);
	}
}

void World::WorldUpdate(float DeltaSeconds)
{

}

void World::DestroyActor(std::shared_ptr<Actor> ActorShared)
{
	if(ActorShared.get())
	{
		DestroyActorByName(ActorShared->GetName());
	}
}

void World::DestroyActorByName(std::string Name)
{
	if(ActorMap.count(Name))
	{
		ActorMap[Name]->BeginDestroy();
		ActorMap.erase(Name);
	}
}

void World::BeginDestroy()
{

}

void World::Destroy()
{
	BeginDestroy();
	ActorMap.clear();	
}

std::string World::CheckActorName(std::string Name)
{
	if (ActorMap.count(Name))
	{
		return BuildActorName(Name, 1);
	}
	return Name;
}

std::string World::BuildActorName(std::string Name, uint32_t BuildCount)
{
	std::string BuildName = Name + "_" + std::to_string(BuildCount);
	if (ActorMap.count(BuildName))
	{
		return BuildActorName(Name, ++BuildCount);
	}
	return BuildName;
}
