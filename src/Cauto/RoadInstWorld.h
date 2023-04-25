#ifndef ROAD_INST_WORLD_H
#define ROAD_INST_WORLD_H

#include "../Core/World.h"
#include "../Math/MathLibrary.h"
#include <memory>
#include <vector>

struct RoadInstCellInfo
{
	uint8_t CellType; // 0.不可走 1.路 2.边缘
	bool bHasCar;
	uint8_t Turn; // 0.没车 1.左转 2.直行 3.右转
	uint8_t Lane; // 进入车道口 0.没车 1.左到右 2.右到左 3.下到上 4.上到下
};

struct RoadInstCarInfo
{
	int Speed;
	std::pair<int, int> CurrPos;
	std::weak_ptr<Actor> CellPtr;
};

struct TrafficLightInfo
{
	int Color; // Color: 0.绿色 1.红色 2.黄色
	std::vector<std::weak_ptr<Actor>> Lights;
};

class RoadInstWorld : public World
{
public:
	RoadInstWorld() = default;
	~RoadInstWorld() = default;

protected:
	virtual void BeginCreate() override;
	virtual void WorldUpdate(float DeltaSeconds) override;
	
private:
	void InitRoadInstWorld();
	void UpdateRoadInstWorld();
	std::pair<int, int> CalcRelativePosition(int Lane, int x, int y, int xOffset, int yOffset);
	bool CalcFinalPosInCrossFork(int Lane, int Turn, const std::pair<int, int>& InCurrPos, std::pair<int, int>& OutFinalPos);
	int CalcFinalPosInStraight(int Lane, int Turn , int CurrSpeed, const std::pair<int, int>& InCurrPos, std::pair<int, int>& OutFinalPos);
	int CalcFinalSpeed(int CurrSpeed, int EmptyOfFront);
	bool IsInCrossForkRoad(int x, int y);
	void SetTrafficLightColor(TrafficLightInfo& TrafficLight, int Color);
	bool CanCrossTrafficLightRoad(int Lane);
	
	float CurrSeconds = 0;
	float TickInterval = 0.2f;
	int RoadWidth = 200;
	int RoadHeight = 120;

	// 车的属性
	int MaxSpeed = 3;
	float SlowDown = 0.2f;

	// 十字路口的范围[6 * 6]
	int CrossWidthInterval[2];
	int CrossHeightInterval[2];
	int ForkHeightInterval[2];

	// 十字路口交通灯 Idx: 0.WidthLight 1.HeightLight Color: 0.绿色 1.红色 2.黄色
	TrafficLightInfo WidthLight;
	TrafficLightInfo HeightLight;

	// 交通灯时间
	int GreenTime = 38;
	int DelayTime = 7;
	int CurrTime = 0;

	std::vector<std::vector<RoadInstCellInfo>> CellSpace;
	std::vector<RoadInstCarInfo> CarArray;
	std::vector<std::weak_ptr<Actor>> Edges;
};

#endif
