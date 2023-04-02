#ifndef WOLFRAM_184_WORLD_H
#define WOLFRAM_184_WORLD_H

#include "../Core/World.h"
#include <memory>
#include <vector>

class Actor;
class Wolfram184World : public World
{
public:
	Wolfram184World() = default;
	~Wolfram184World() = default;
protected:
	virtual void BeginCreate() override;
	virtual void WorldUpdate(float DeltaSeconds) override;
	virtual void BeginDestroy() override;

private:
	void WolframInit();
	void WolframUpdate();
	bool CheckPosHasCell(uint8_t CellSpaceIndex, int x) const;
	
	// 世界更新频率
	float CurrSeconds = 0.f;
	float TickInterval = 0.1f;

    int RoadLength = 256;

	// 元胞空间,单车道模型
	uint8_t CurrCellSpaceIndex;
	std::vector<bool> CellSpace[2];
	std::vector<std::weak_ptr<Actor>> CellArray;

};

#endif
