#ifndef CA_CAMERA_H
#define CA_CAMERA_H

#include "../Core/Camera.h"

class CACamera : public Camera
{
private:
	void EnterNextWorld();
	virtual void SetUpInput(InputSystem* MainInput) override;

	static int WorldIdx;
};

#endif
