#include "Wolfram184World.h"
#include "../Core/Actor.h"
#include <string>

void Wolfram184World::BeginCreate()
{
	World::BeginCreate();
	WolframInit();
}

void Wolfram184World::WorldUpdate(float DeltaSeconds)
{
	World::WorldUpdate(DeltaSeconds);
	CurrSeconds += DeltaSeconds;
	if(CurrSeconds > TickInterval)
	{
		CurrSeconds = 0;
		WolframUpdate();
	}	
}

void Wolfram184World::BeginDestroy()
{
	World::BeginDestroy();
}

void Wolfram184World::WolframInit()
{
	CurrCellSpaceIndex = 0;
	for(int i = 0; i < 2; ++i)
	{
		CellSpace[i] = std::vector<bool>(RoadLength, false);	
	}
	CellArray.resize(RoadLength);

	int YStart = - (RoadLength >> 1);

	auto& InitCellSpace = CellSpace[CurrCellSpaceIndex];
	for(int i = 0; i < RoadLength; ++i)
	{
		InitCellSpace[i] = !(rand() % 2);
		if(InitCellSpace[i])
			CellArray[i] = SpawnActor<Actor>(std::to_string(i), Vector3(0.f, i + YStart, 0.f));
	}

}

void Wolfram184World::WolframUpdate()
{
	int YStart = -(RoadLength / 2);

	uint8_t LastCellSpaceIndex = CurrCellSpaceIndex;
	CurrCellSpaceIndex = ++CurrCellSpaceIndex % 2;
	auto& LastCellSpace = CellSpace[LastCellSpaceIndex];
	auto& CurrCellSpace = CellSpace[CurrCellSpaceIndex];

	for(int i = 0; i < RoadLength; i++)
	{
		// Wolfram184模型
		if(LastCellSpace[i])
		{
			CurrCellSpace[i] = CheckPosHasCell(LastCellSpaceIndex, i + 1);
		}
		else	
	   	{
			CurrCellSpace[i] = CheckPosHasCell(LastCellSpaceIndex, i - 1);
		}
			
		// 更新Actor(细胞)
		if(CurrCellSpace[i])
		{
			if(CellArray[i].expired())
			{
				CellArray[i] = SpawnActor<Actor>(std::to_string(i), Vector3(0.f, i + YStart, 0.f));
			}
		}
		else
	   	{
			if(!CellArray[i].expired())	
			{
				DestroyActor(CellArray[i].lock());
			}	
		}
	}
}

bool Wolfram184World::CheckPosHasCell(uint8_t CellSpaceIndex, int x) const
{
	return CellSpaceIndex >= 0 && CellSpaceIndex < 2 && x >= 0 && x < RoadLength && CellSpace[CellSpaceIndex][x];
}
