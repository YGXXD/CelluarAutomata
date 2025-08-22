#ifndef BIRTH_2025_CAMERA_H
#define BIRTH_2025_CAMERA_H

#include "../Core/Camera.h"

class Birth2025Camera : public Camera
{
private:
	void EnterNextWorld();
	virtual void SetUpInput(InputSystem* MainInput) override;
};

#endif
