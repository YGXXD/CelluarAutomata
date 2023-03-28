#pragma once
#include "../Core/World.h"
#include <vector>

enum class NeighborType : uint8_t
{
	MooreArea,
	VonNeumannArea,
	MaxValue
};

class LifeGame3DWorld : public World
{
public:
	LifeGame3DWorld();
	~LifeGame3DWorld();
protected:
	virtual void BeginCreate() override;
	virtual void WorldUpdate(float DeltaSeconds) override;
	virtual void BeginDestroy() override;
private:
	void GameOfLifeInit();
	void GameOfLifeUpdate(); 
	uint8_t FindCellNearNum(uint8_t CellSpaceIndex,int x, int y, int z);
	bool CheckPosHasCell(uint8_t CellSpaceIndex, int x, int y, int z);

	float CurrSeconds = 0.f;
	float TickInterval = 0.01f;
	int SpaceLength = 100;
	int SpaceWidth = 100;
	int SpaceHeight = 100;

	// 摩尔邻域
	std::vector<uint8_t> Survival = {4};
	std::vector<uint8_t> Birth = {4}; 
	uint8_t State = 5;
	NeighborType Neighbor = NeighborType::MooreArea;
	Vector3 MaxStateColor = Vector3(1, 1, 1);
	Vector3 MinStateColor = Vector3(0.4, 0.4, 0.4);

	// 三维数组
	uint8_t CurrCellSpaceIndex = 0;
	std::vector<std::vector<std::vector<uint8_t>>> CellSpace[2];
	std::vector<std::vector<std::vector<std::weak_ptr<Actor>>>> CellArray;
};
