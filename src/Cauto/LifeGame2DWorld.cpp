#include "LifeGame2DWorld.h"
#include <algorithm>
#include "CACamera.h"

LifeGame2DWorld::LifeGame2DWorld() : World()
{

}

LifeGame2DWorld::~LifeGame2DWorld()
{

}

void LifeGame2DWorld::BeginCreate()
{
	World::BeginCreate();
	SpawnActor<CACamera>("CACamera");
	GameOfLifeInit();		
}

void LifeGame2DWorld::WorldUpdate(float DeltaSeconds)
{
	World::WorldUpdate(DeltaSeconds);
	CurrSeconds += DeltaSeconds;
	if(CurrSeconds >= TickInterval)
	{
		GameOfLifeUpdate();
		CurrSeconds = 0.f;
	}
	
}

void LifeGame2DWorld::BeginDestroy()
{
	World::BeginDestroy();
}

void LifeGame2DWorld::GameOfLifeInit()
{
	std::sort(Survival.begin(), Survival.end());
	std::sort(Birth.begin(), Birth.end());

	CurrCellSpaceIndex = 0;
	for(int i = 0; i < 2; i++)
	{
		CellSpace[i] = std::vector<std::vector<bool>>(SpaceWidth, std::vector<bool>(SpaceHeight, false));
	}
	CellArray = std::vector<std::vector<std::weak_ptr<Actor>>>(SpaceWidth, std::vector<std::weak_ptr<Actor>>(SpaceHeight));
	
	int XStart = -(SpaceWidth / 2);
	int YStart = -(SpaceHeight / 2);
	for(int i = 0; i < SpaceWidth; i++)
	{
		for(int j = 0; j < SpaceHeight; j++)
		{
			// 初始化五分之一概率生成细胞
			bool BirthProbability = (rand() % 5) == 0;
			if(BirthProbability)
			{
				CellSpace[CurrCellSpaceIndex][i][j] = true;
				std::string Name = std::to_string(i) + "," + std::to_string(j);
				CellArray[i][j] = SpawnActor<Actor>(Name, Vector3(0, XStart + i, YStart + j));
				CellArray[i][j].lock()->SetRenderColor(CellColor);
			}
		}
	}
}

void LifeGame2DWorld::GameOfLifeUpdate()
{
	int XStart = -(SpaceWidth / 2);
	int YStart = -(SpaceHeight / 2);

	uint8_t LastCellSpaceIndex = CurrCellSpaceIndex;
	CurrCellSpaceIndex = ++CurrCellSpaceIndex % 2;
	auto& LastCellSpace = CellSpace[LastCellSpaceIndex];
	auto& CurrCellSpace = CellSpace[CurrCellSpaceIndex];

	for(int i = 0; i < SpaceWidth; i++)
	{
		for(int j = 0; j < SpaceHeight; j++)
		{
			uint8_t NearNum = FindCellNearNum(LastCellSpaceIndex, i, j);
			
			// Survival
			if(LastCellSpace[i][j])
			{
				CurrCellSpace[i][j] = std::binary_search(Survival.begin(), Survival.end(), NearNum);
			}
			// Birth
			else
			{
				CurrCellSpace[i][j] = std::binary_search(Birth.begin(), Birth.end(), NearNum);
			}
			
			// 更新Actor(细胞)
			if(CurrCellSpace[i][j])
			{
				if(CellArray[i][j].expired())
				{
					std::string Name = std::to_string(i) + "," + std::to_string(j);
					CellArray[i][j] = SpawnActor<Actor>(Name, Vector3(0, XStart + i, YStart + j));
					CellArray[i][j].lock()->SetRenderColor(CellColor);
				}
			}
			else
		   	{
				if(!CellArray[i][j].expired())	
				{
					DestroyActor(CellArray[i][j].lock());
				}	
			}
		}
	}
}

uint8_t LifeGame2DWorld::FindCellNearNum(uint8_t CellSpaceIndex, int x, int y)
{
	uint8_t Num = 0;
		
	if(CheckPosHasCell(CellSpaceIndex, x + 1, y))
		Num++;
	if(CheckPosHasCell(CellSpaceIndex, x + 1, y + 1))
		Num++;
	if(CheckPosHasCell(CellSpaceIndex, x + 1, y - 1))
		Num++;
	if(CheckPosHasCell(CellSpaceIndex, x, y + 1))
		Num++;	
	if(CheckPosHasCell(CellSpaceIndex, x, y - 1))
		Num++;
	if(CheckPosHasCell(CellSpaceIndex, x - 1, y))
		Num++;
	if(CheckPosHasCell(CellSpaceIndex, x - 1, y + 1))
		Num++;
	if(CheckPosHasCell(CellSpaceIndex, x - 1, y - 1))
		Num++;

	return Num;
}

bool LifeGame2DWorld::CheckPosHasCell(uint8_t CellSpaceIndex, int x, int y)
{
	return CellSpaceIndex >=0 && CellSpaceIndex < 2 && x >=0 && x < SpaceWidth && y >=0 && y < SpaceHeight && CellSpace[CellSpaceIndex][x][y];
}
