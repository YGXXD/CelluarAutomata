#ifndef LIFEGAME_2D_WORLD_H
#define LIFEGAME_2D_WORLD_H

#include "../Core/World.h"
#include <vector>

class LifeGame2DWorld : public World
{
public:
	LifeGame2DWorld();
	~LifeGame2DWorld();
protected:
	virtual void BeginCreate() override;
	virtual void WorldUpdate(float DeltaSeconds) override;
	virtual void BeginDestroy() override;
private:
	void GameOfLifeInit();
	void GameOfLifeUpdate(); 
	uint8_t FindCellNearNum(uint8_t CellSpaceIndex,int x, int y);
	bool CheckPosHasCell(uint8_t CellSpaceIndex, int x, int y);

	float CurrSeconds = 0.f;
	float TickInterval = 0.2f;
	int SpaceWidth = 100;
	int SpaceHeight = 100;

	// 摩尔邻域
	std::vector<uint8_t> Survival = {2, 3}; 
	std::vector<uint8_t> Birth = {3};
	Vector3 CellColor = Vector3(1, 1, 1);

	uint8_t CurrCellSpaceIndex = 0;
	std::vector<std::vector<bool>> CellSpace[2];
	std::vector<std::vector<std::weak_ptr<Actor>>> CellArray;
};

#endif
