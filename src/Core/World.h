#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <vector>
#include <unordered_map>
#include "Actor.h"
#include "../Render/RenderInterface.h"

class Camera;
class World : public IRenderWorld
{
public:
	World();
	World(World& Copy) = delete;
	World& operator= (World& Copy) = delete;
	virtual ~World();

	void Create();
	virtual void BeginCreate();

	void Update(float DeltaSeconds);
	virtual void WorldUpdate(float DeltaSeconds);

	template<class T>
	std::weak_ptr<T> SpawnActor(std::string Name, Vector3 Location = Vector3(), Vector3 Scale = Vector3(1, 1, 1));

	void DestroyActor(std::shared_ptr<Actor> ActorShared);
	void DestroyActorByName(std::string Name);

	template<class T>
	std::weak_ptr<T> GetActorFromClass();
	
	virtual void BeginDestroy();

	void Destroy();
private:
	virtual void Draw(IRenderDraw* pDraw) override;
	virtual void UpdateCameraDataPak(CameraDataPak* pDataPak) override;

	std::string CheckActorName(std::string Name);	
	std::string BuildActorName(std::string Name, uint32_t BuildCount);

	std::unordered_map<std::string, std::shared_ptr<Actor>> ActorMap;

	std::weak_ptr<Camera> CameraWeak;
};

template<class T>
std::weak_ptr<T> World::SpawnActor(std::string Name, Vector3 Location, Vector3 Scale)
{
	Name = CheckActorName(Name);
	std::shared_ptr<Actor> ActorShared = std::make_shared<T>();
	ActorMap[Name] = ActorShared;
	ActorShared->SetName(Name);
	ActorShared->SetActorLocation(Location);
	ActorShared->SetActorScale(Scale);
	ActorShared->BeginPlay();

	return std::dynamic_pointer_cast<T>(ActorShared);
}

template<class T>
std::weak_ptr<T> World::GetActorFromClass()
{
	for(auto& ActorInfo : ActorMap)
	{
		std::shared_ptr<T> TShared = std::dynamic_pointer_cast<T>(ActorInfo.second);
		if(TShared.get())
			return TShared;
	}

	return std::weak_ptr<T>();
}

#endif
