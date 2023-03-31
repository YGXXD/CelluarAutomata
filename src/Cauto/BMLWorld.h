#pragma once
#include "../Core/World.h"

enum class EBMLDirection : uint8_t
{
	Right,
	Left,
	Up,
	Down,
	Max // Max代表没车
};

class BMLWorld : public World
{
public:
	BMLWorld() = default;
	~BMLWorld() = default;

protected:
	virtual void BeginCreate() override;
	virtual void WorldUpdate(float DeltaSeconds) override;

private:
	void InitBMLWorld();
	void UpdateBMLWorld();
	bool CheckPosHasCell(uint8_t CellSpaceIndex, int& x, int& y); //返回坐标

	float CurrSeconds = 0;
	float TickInterval = 0.1f;

	// 地图宽高
	int RoadWidth = 80;
	int RoadHeight = 80;

	std::unordered_map<uint8_t, Vector3> ColorMap = 
	{
		{static_cast<uint8_t>(EBMLDirection::Right), Vector3(1, 0, 0)},
		{static_cast<uint8_t>(EBMLDirection::Left), Vector3(0, 1, 0)},
		{static_cast<uint8_t>(EBMLDirection::Up), Vector3(0, 0, 1)},
		{static_cast<uint8_t>(EBMLDirection::Down), Vector3(1, 1, 1)}
	};

	// 元胞空间,BML模型
	uint8_t CurrDirection;
	uint8_t CurrCellSpaceIndex;
	std::vector<std::vector<EBMLDirection>> CellSpace[2];
	std::vector<std::vector<std::weak_ptr<Actor>>> CellArray;
};
