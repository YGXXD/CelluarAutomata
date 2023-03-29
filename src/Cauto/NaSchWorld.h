#pragma once
#include "../Core/World.h"

struct CellInfo
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
	int GetEmptyFront(int CellSpaceIndex, int x, int y);

	float CurrSeconds = 0.f;
	float TickInterval = 1.f;

    int RoadLength = 200;
	int RoadWidth = 1;

	float SlowDown = 0.3f; //慢随机化概率
	int MaxSpeed = 5;

	// 元胞空间,单车道模型
	uint8_t CurrCellSpaceIndex;
	std::vector<std::vector<CellInfo>> CellSpace[2];
	std::vector<std::vector<std::weak_ptr<Actor>>> CellArray;
};
