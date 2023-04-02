#ifndef CAMERA_H
#define CAMERA_H

#include "Actor.h"

class Camera : public Actor 
{
public:
	Camera();

	float GetFOV() const;
	float GetNearZ() const;
	float GetFarZ() const;
	Vector3 GetFocus() const;

protected:
	virtual void BeginPlay() override;
   	virtual void Update(float DeltaSeconds) override;	
	virtual void SetUpInput(InputSystem* MainInput) override;

private:
	void AddPitch(float DeltaSeconds);
	void SubPitch(float DeltaSeconds);
	void AddYaw(float DeltaSeconds);
	void SubYaw(float DeltaSeconds);
	void AddRadius(float DeltaSeconds);
	void SubRadius(float DeltaSeconds);

	float Radius = 100.f;
	float Pitch = 0.f;
	float Yaw = 3.14159f;

	Vector3 Focus;
	float FOV;
	float NearZ;
	float FarZ;
};

#endif
