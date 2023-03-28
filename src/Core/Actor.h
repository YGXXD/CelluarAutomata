#pragma once
#include "../Math/Vector3.h"
#include <string>

class World;
class InputSystem;
class Actor
{
public:
	Actor();
	Actor(Actor& Copy) = delete;
	Actor& operator=(Actor& Copy) = delete;
	virtual ~Actor();

	World* GetWorld();
	std::string GetName() const;
	void SetName(std::string NewName);
	void SetRenderColor(Vector3 NewColor);

	bool IsUpdate() const;
	bool IsHidden() const;
	std::string GetGeometryName() const;
	std::string GetPipelineName() const;
	Vector3 GetRenderColor() const;

	virtual void BeginPlay();
	virtual void Update(float DeltaSeconds);
	virtual void BeginDestroy();

	virtual void SetUpInput(InputSystem* MainInput);

	Vector3 GetActorLocation() const;
	Vector3 GetActorScale() const;
	void SetActorLocation(Vector3 NewLocation);
	void SetActorScale(Vector3 NewScale);
protected:
	bool bIsUpdate;
	bool bIsHidden;
	std::string GeometryName;
	std::string PipelineName;
	Vector3 RenderColor;

private:
	bool bNameLock;
	std::string Name;

	Vector3 Location;
	Vector3 Scale;
};
