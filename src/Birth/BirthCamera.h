#ifndef BIRTH_CAMERA_H
#define BIRTH_CAMERA_H

#include "../Core/Camera.h"

class BirthCamera : public Camera
{
private:
	void EnterNextWorld();
	virtual void SetUpInput(InputSystem* MainInput) override;

	static int WorldIdx;
};

#endif
