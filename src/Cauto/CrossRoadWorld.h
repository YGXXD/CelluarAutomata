#ifndef CROSS_ROAD_WORLD_H
#define CROSS_ROAD_WORLD_H

#include "../Core/World.h"
#include "../Math/MathLibrary.h"
#include <memory>
#include <vector>

struct CrossRoadCellInfo
{
	uint8_t CellType; // 0.不可走 1.路 2.边缘
	bool bHasCar;
	uint8_t Turn; // 0.没车 1.左转 2.直行 3.右转
	uint8_t Lane; // 进入车道口 0.没车 1.左到右 2.右到左 3.下到上 4.上到下
};

struct CrossRoadCarInfo
{
	int Speed;
	std::pair<int, int> CurrPos;
	std::weak_ptr<Actor> CellPtr;
};

using std::vector;

class CrossRoadWorld : public World
{
public:
	CrossRoadWorld() = default;
	~CrossRoadWorld() = default;

protected:
	virtual void BeginCreate() override;
	virtual void WorldUpdate(float DeltaSeconds) override;
	
private:
	void InitCrossRoadWorld();
	void UpdateCrossRoadWorld();
	std::pair<int, int> CalcRelativePosition(int Lane, int x, int y, int xOffset, int yOffset);
	bool CalcFinalPos(int Lane, int Turn, const std::pair<int, int>& InCurrPos, std::pair<int, int>& OutFinalPos);
	bool IsInCrossCenter(int x, int y);
	
	float CurrSeconds = 0;
	float TickInterval = 0.1f;
	int RoadWidth = 200;
	int RoadHeight = 120;

	// 车的属性
	// int MaxSpeed = 5;
	// float SlowDown = 0.2f;

	// 十字路口的范围[6 * 6]
	int CrossWidthInterval[2];
	int CrossHeightInterval[2];

	vector<vector<CrossRoadCellInfo>> CellSpace;
	vector<CrossRoadCarInfo> CarArray;
	std::vector<std::weak_ptr<Actor>> Edges;
};

#endif
