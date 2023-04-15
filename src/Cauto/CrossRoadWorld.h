#ifndef CROSS_ROAD_WORLD_H
#define CROSS_ROAD_WORLD_H

#include "../Core/World.h"

enum class CrossRoadGridType : uint8_t
{
	Road,
	Edge,
	Max //Max代表没有格子
};

enum class CrossRoadDirection : uint8_t
{
	Up,
	Down,
	Right,
	Left,
	Max //Max代表十字路口中间
};

enum class CrossRoadLightColor : uint8_t
{
	Green,
	Yellow,
	Red
};

struct CrossRoadGrid
{
	bool bHasCell;
	CrossRoadGridType GridType;
	CrossRoadDirection GridDirection;
};

struct CrossRoadCellInfo
{
	CrossRoadDirection CurrDirection;	
	float CurrSpeed;
	std::pair<int, int> CurrPosition;
	std::weak_ptr<Actor> CellPtr;		
};

class CrossRoadWorld : public World
{
public:
	CrossRoadWorld() = default;
	~CrossRoadWorld() = default;

private:
	virtual void BeginCreate() override;
	virtual void WorldUpdate(float DeltaSeconds) override;

private:
	void InitCrossRoadWorld();
	void UpdateCrossRoadWorld();

	void SetWidthLightColor(CrossRoadLightColor Color);
	void SetHeightLightColor(CrossRoadLightColor Color);
	int GetEmptyFront(CrossRoadDirection Direction, int x, int y);
	int CalcSpeed(int CurrSpeed, int EmptyOfFront);
	std::pair<int, int> CalcPos(const std::pair<int, int>& CurrPos, int Speed, CrossRoadDirection Direction);
	bool IsInCross(int x, int y);
	bool CanCross(CrossRoadDirection Direction);
	
	float CurrSeconds = 0.f;
	float TickInterval = 0.2f;
	int RoadWidth = 120;
	int RoadHeight = 80;

	// 车的属性
	int MaxSpeed = 5;
	float SlowDown = 0.2f;

	// 交通灯时间
	int GreenTime = 18;
	int DelayTime = 5;
	int CurrTime = 0;
	
	// 十字路口的范围
	int CrossWidthInterval[2];
	int CrossHeightInterval[2];
	std::vector<std::weak_ptr<Actor>> WidthLights;
	std::vector<std::weak_ptr<Actor>> HeightLights;
	CrossRoadLightColor WidthLightColor;
	CrossRoadLightColor HeightLightColor;

	std::vector<std::vector<CrossRoadGrid>> GridSpace;
	std::vector<CrossRoadCellInfo> CarArray;
	std::vector<std::weak_ptr<Actor>> Edges;

	// std::vector<std::vector<std::weak_ptr<Actor>>> CellArray;
};

#endif
