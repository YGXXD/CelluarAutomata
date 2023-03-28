#include "Camera.h"
#include "../Math/MathLibrary.h"
#include "InputSystem.h"
#include <SDL_scancode.h>

Camera::Camera() : Actor()
{
	bIsUpdate = true;
	bIsHidden = true;
	GeometryName = "Camera";
	PipelineName = "CameraPipeline";

	Focus = Vector3();
	FOV = 45;
   	NearZ = 2;
	FarZ = 150;	
}

float Camera::GetFOV() const
{
	return FOV;
}

float Camera::GetNearZ() const
{
	return NearZ;
}

float Camera::GetFarZ() const
{
	return FarZ;
}

Vector3 Camera::GetFocus() const
{
	return Focus;
}

void Camera::BeginPlay()
{
	Actor::BeginPlay();
	SetActorLocation(Vector3(Radius * Math::Cos(Pitch) * Math::Cos(Yaw), Radius * Math::Cos(Pitch) * Math::Sin(Yaw), Radius * Math::Sin(Pitch)));
}

void Camera::Update(float DeltaSeconds)
{
	Actor::Update(DeltaSeconds);

	SetActorLocation(Vector3(Radius * Math::Cos(Pitch) * Math::Cos(Yaw), Radius * Math::Cos(Pitch) * Math::Sin(Yaw), Radius * Math::Sin(Pitch)));
}

void Camera::SetUpInput(InputSystem* MainInput)
{
	MainInput->AxisBind(SDL_SCANCODE_W, this, &Camera::AddPitch);
	MainInput->AxisBind(SDL_SCANCODE_S, this, &Camera::SubPitch);
	MainInput->AxisBind(SDL_SCANCODE_A, this, &Camera::AddYaw);
	MainInput->AxisBind(SDL_SCANCODE_D, this, &Camera::SubYaw);
	MainInput->AxisBind(SDL_SCANCODE_I, this, &Camera::AddRadius);
	MainInput->AxisBind(SDL_SCANCODE_K, this, &Camera::SubRadius);
}

void Camera::AddPitch(float DeltaSeconds)
{
	Pitch = Math::Clamp(-1.5f, 1.5f, Pitch + DeltaSeconds);
}

void Camera::SubPitch(float DeltaSeconds)
{
	Pitch = Math::Clamp(-1.5f, 1.5f, Pitch - DeltaSeconds);
}

void Camera::AddYaw(float DeltaSeconds)
{
	Yaw += DeltaSeconds;
}

void Camera::SubYaw(float DeltaSeconds)
{
	Yaw -= DeltaSeconds;
}

void Camera::AddRadius(float DeltaSeconds)
{
	Radius = Math::Clamp(NearZ, FarZ, Radius + 10 * DeltaSeconds);
}

void Camera::SubRadius(float DeltaSeconds)
{
	Radius = Math::Clamp(NearZ, FarZ, Radius - 10 * DeltaSeconds);
}
