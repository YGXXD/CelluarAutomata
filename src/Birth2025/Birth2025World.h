#ifndef BIRTH_2025_WORLD_H
#define BIRTH_2025_WORLD_H

#include "../Core/World.h"
#include "../Core/World.h"
#include "../Birth/BirthActor.h"
#include <vector>

struct Birth2025ActorContent
{
	
	std::vector<std::vector<std::weak_ptr<BirthActor>>> CellArray;
};

class Birth2025World : public World
{
public:
	Birth2025World();
	~Birth2025World();
protected:
	virtual void BeginCreate() override;
	virtual void WorldUpdate(float DeltaSeconds) override;
	virtual void BeginDestroy() override;
private:
	void BirthDayInit(int Width, int Height, float ScreenX, 
		float ScreenY, const std::string& Path, bool RenderAsText, float Scale = 0.5f);
	void SpawnImageActor();

	float LangdingStartTime = 0.f;
	float CellStartInterval = 0.001f;
	uint32_t CellIndex = 0;
	Vector3 CellColor = Vector3(1.f, 1.f, 1.f);
	std::vector<std::weak_ptr<BirthActor>> CellArray;
};

#endif
