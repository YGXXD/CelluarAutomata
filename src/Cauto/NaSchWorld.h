#ifndef NASCH_WORLD_H
#define NASCH_WORLD_H

#include "../Core/World.h"

struct NaSchCellInfo
{
	bool bHasCar = 0;
	int Speed = 0;
};

class NaSchWorld : public World
{
public:
	NaSchWorld() = default;
	~NaSchWorld() = default;

protected:
	
	virtual void BeginCreate() override;
	virtual void WorldUpdate(float DeltaSeconds) override;
	
private:
	void InitNaSchWorld();
	void UpdateNaSchWorld();
	int GetEmptyFront(uint8_t CellSpaceIndex, int x, int y);

	float CurrSeconds = 0.f;
	float TickInterval = 0.1f;

    int RoadLength = 200;
	int RoadWidth = 1;

	float SlowDown = 0.3f; //慢随机化概率
	int MaxSpeed = 3;

	// 元胞空间,单车道模型
	uint8_t CurrCellSpaceIndex;
	std::vector<std::vector<NaSchCellInfo>> CellSpace[2];
	std::vector<std::vector<std::weak_ptr<Actor>>> CellArray;
};

#endif
