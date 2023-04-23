#ifndef TRAFFIC_LIGHT_WORLD_H
#define TRAFFIC_LIGHT_WORLD_H

#include "../Core/World.h"

enum class TrafficLightGridType : uint8_t
{
	Road,
	Edge,
	Max //Max代表没有格子
};

enum class TrafficLightDirection : uint8_t
{
	Up,
	Down,
	Right,
	Left,
	Max //Max代表十字路口中间
};

enum class TrafficLightColor : uint8_t
{
	Green,
	Yellow,
	Red
};

struct TrafficLightGrid
{
	bool bHasCell;
	TrafficLightGridType GridType;
	TrafficLightDirection GridDirection;
};

struct TrafficLightCellInfo
{
	TrafficLightDirection CurrDirection;	
	int CurrSpeed;
	std::pair<int, int> CurrPosition;
	std::weak_ptr<Actor> CellPtr;		
};

class TrafficLightWorld : public World
{
public:
	TrafficLightWorld() = default;
	~TrafficLightWorld() = default;

protected:
	virtual void BeginCreate() override;
	virtual void WorldUpdate(float DeltaSeconds) override;

private:
	void InitTrafficLightWorld();
	void UpdateTrafficLightWorld();

	void SetWidthLightColor(TrafficLightColor Color);
	void SetHeightLightColor(TrafficLightColor Color);
	int GetEmptyFront(TrafficLightDirection Direction, int x, int y);
	int CalcSpeed(int CurrSpeed, int EmptyOfFront);
	std::pair<int, int> CalcPos(const std::pair<int, int>& CurrPos, int Speed, TrafficLightDirection Direction);
	bool IsInCross(int x, int y);
	bool CanCross(TrafficLightDirection Direction);
	
	float CurrSeconds = 0.f;
	float TickInterval = 0.2f;
	int RoadWidth = 200;
	int RoadHeight = 120;

	// 车的属性
	int MaxSpeed = 5;
	float SlowDown = 0.2f;

	// 交通灯时间
	int GreenTime = 38;
	int DelayTime = 7;
	int CurrTime = 0;
	
	// 十字路口的范围
	int CrossWidthInterval[2];
	int CrossHeightInterval[2];
	std::vector<std::weak_ptr<Actor>> WidthLights;
	std::vector<std::weak_ptr<Actor>> HeightLights;
	TrafficLightColor WidthLightColor;
	TrafficLightColor HeightLightColor;

	std::vector<std::vector<TrafficLightGrid>> GridSpace;
	std::vector<TrafficLightCellInfo> CarArray;
	std::vector<std::weak_ptr<Actor>> Edges;

	// std::vector<std::vector<std::weak_ptr<Actor>>> CellArray;
};

#endif
