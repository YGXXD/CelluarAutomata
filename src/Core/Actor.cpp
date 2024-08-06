#include "Actor.h"
#include "Application.h"
#include "InputSystem.h"
#include "World.h"
#include <iostream>

Actor::Actor()
{	
	bIsUpdate = false;
	bIsHidden = false;
	bNameLock = false;
	GeometryName = "Box";
	PipelineName = "BoxPipeline";
	RenderColor = Vector3(1, 1, 0);
}

Actor::~Actor() 
{
}

World* Actor::GetWorld()
{
	return Application::GetMainWorld().lock().get();
}

std::string Actor::GetName() const
{
	return Name;
}

void Actor::SetName(std::string NewName) 
{
	if(!bNameLock)
	{
		Name = NewName;
		bNameLock = true;
	}
	else
	{
		std::cout << "Don't Modify Actor's Name (" << Name << ")!" << std::endl;
	}
}

void Actor::SetIsUpdate(bool IsUpdate)
{
	bIsUpdate = IsUpdate;
}

void Actor::SetRenderColor(Vector3 NewColor)
{
	RenderColor = NewColor;
}

std::string Actor::GetGeometryName() const
{
	return GeometryName;
}

std::string Actor::GetPipelineName() const
{
	return PipelineName;
}

Vector3 Actor::GetRenderColor() const
{
	return RenderColor;
}

bool Actor::IsUpdate() const
{
	return bIsUpdate;
}

bool Actor::IsHidden() const
{
	return bIsHidden;
}

Vector3 Actor::GetActorLocation() const
{
	return Location;
}

Vector3 Actor::GetActorScale() const
{
	return Scale;
}

void Actor::SetActorLocation(Vector3 NewLocation)
{
	Location = NewLocation;
}

void Actor::SetActorScale(Vector3 NewScale)
{
	Scale = NewScale;
}

void Actor::BeginPlay()
{

}

void Actor::Update(float DeltaSeconds)
{

}

void Actor::BeginDestroy()
{

}

void Actor::SetUpInput(InputSystem* MainInput)
{
	
}
