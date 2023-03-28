#include "LifeGame3DWorld.h"
#include <algorithm>
#include <iostream>
#include "../Math/MathLibrary.h"

LifeGame3DWorld::LifeGame3DWorld() : World()
{

}

LifeGame3DWorld::~LifeGame3DWorld()
{

}

void LifeGame3DWorld::BeginCreate()
{
	World::BeginCreate();
	GameOfLifeInit();		
}

void LifeGame3DWorld::WorldUpdate(float DeltaSeconds)
{
	World::WorldUpdate(DeltaSeconds);
	CurrSeconds += DeltaSeconds;
	if(CurrSeconds >= TickInterval)
	{
		GameOfLifeUpdate();
		CurrSeconds = 0.f;
	}
}

void LifeGame3DWorld::BeginDestroy()
{
	World::BeginDestroy();
}

void LifeGame3DWorld::GameOfLifeInit()
{
	std::sort(Survival.begin(), Survival.end());
	std::sort(Birth.begin(), Birth.end());

	CurrCellSpaceIndex = 0;
	for(int i = 0; i < 2; i++)
	{
		CellSpace[i] = std::vector<std::vector<std::vector<uint8_t>>>(SpaceLength, std::vector<std::vector<uint8_t>>(SpaceWidth, std::vector<uint8_t>(SpaceHeight, 0)));
	}
	CellArray = std::vector<std::vector<std::vector<std::weak_ptr<Actor>>>>(SpaceLength, std::vector<std::vector<std::weak_ptr<Actor>>>(SpaceWidth, std::vector<std::weak_ptr<Actor>>(SpaceHeight)));

	
	int XStart = -(SpaceLength / 2);
	int YStart = -(SpaceWidth / 2);
	int ZStart = -(SpaceHeight / 2);

/*	
	int i = SpaceLength / 2;
	int j = SpaceWidth / 2;
	int k = SpaceHeight / 2;
	CellSpace[CurrCellSpaceIndex][i][j][k] = State;
	std::string Name = std::to_string(i) + "," + std::to_string(j) + "," + std::to_string(k);
	CellArray[i][j][k] = SpawnActor<Actor>(Name, Vector3(XStart + i, YStart + j, ZStart + k));
	CellArray[i][j][k].lock()->SetRenderColor(MaxStateColor);
*/
	for(int i = 0; i < SpaceLength; i++)
	{
		for(int j = 0; j < SpaceWidth; j++)
		{
			for(int k = 0; k < SpaceHeight; k++)
			{
				// 初始化生成细胞
				bool BirthProbability = rand() % 25 == 0;
				if(BirthProbability)
				{
					CellSpace[CurrCellSpaceIndex][i][j][k] = State;
					std::string Name = std::to_string(i) + "," + std::to_string(j) + "," + std::to_string(k);
					CellArray[i][j][k] = SpawnActor<Actor>(Name, Vector3(XStart + i, YStart + j, ZStart + k));
					CellArray[i][j][k].lock()->SetRenderColor(MaxStateColor);
				}
			}
		}
	}
}

void LifeGame3DWorld::GameOfLifeUpdate()
{
	int XStart = -(SpaceLength / 2);
	int YStart = -(SpaceWidth / 2);
	int ZStart = -(SpaceHeight / 2);

	uint8_t LastCellSpaceIndex = CurrCellSpaceIndex;
	CurrCellSpaceIndex = ++CurrCellSpaceIndex % 2;
	auto& LastCellSpace = CellSpace[LastCellSpaceIndex];
	auto& CurrCellSpace = CellSpace[CurrCellSpaceIndex];

	for(int i = 0; i < SpaceLength; i++)
	{
		for(int j = 0; j < SpaceWidth; j++)
		{
			for(int k = 0; k < SpaceHeight; k++)
			{
				uint8_t NearNum = FindCellNearNum(LastCellSpaceIndex, i, j, k);
				// Survival
				if(LastCellSpace[i][j][k] == State)
				{
					CurrCellSpace[i][j][k] = std::binary_search(Survival.begin(), Survival.end(), NearNum) ? State : State - 1;
				}
				else if (LastCellSpace[i][j][k])
			   	{
					CurrCellSpace[i][j][k] = LastCellSpace[i][j][k] - 1;
				}
				// Birth
				else if(!LastCellSpace[i][j][k])
				{
					CurrCellSpace[i][j][k] = std::binary_search(Birth.begin(), Birth.end(), NearNum) ? State : 0;
				}

				uint8_t TempState = CurrCellSpace[i][j][k];
				
				// 更新Actor(细胞)
				if(TempState)
				{
					if(CellArray[i][j][k].expired())
					{
						std::string Name = std::to_string(i) + "," + std::to_string(j) + "," + std::to_string(k);
						CellArray[i][j][k] = SpawnActor<Actor>(Name, Vector3(XStart + i, YStart + j, ZStart + k));
					}
					float LinearValue = State == 1 ? 1 : (float)(TempState - 1) / (State - 1);
					CellArray[i][j][k].lock()->SetRenderColor(Math::Linear(MinStateColor, MaxStateColor, LinearValue));
				}
				else
		   		{
					if(!CellArray[i][j][k].expired())	
					{
						DestroyActor(CellArray[i][j][k].lock());
					}	
				}
			}
		}
	}
}

uint8_t LifeGame3DWorld::FindCellNearNum(uint8_t CellSpaceIndex, int x, int y, int z)
{
	uint8_t Num = 0;

	switch (Neighbor)
   	{
		case NeighborType::MooreArea:
			for(int i = -1; i < 2; i++)
			{
				for(int j = -1; j < 2; j++)
				{
					for(int k = -1; k < 2; k++)
					{
						if(i == 0 && j == 0 && k == 0)
							continue;
						if(CheckPosHasCell(CellSpaceIndex, x + i, y + j, z + k))
							Num++;
					}
				}
			}
			break;
		case NeighborType::VonNeumannArea:
			if(CheckPosHasCell(CellSpaceIndex, x, y, z + 1))
				Num++;
			if(CheckPosHasCell(CellSpaceIndex, x, y, z - 1))
				Num++;
			if(CheckPosHasCell(CellSpaceIndex, x, y + 1, z))
				Num++;
			if(CheckPosHasCell(CellSpaceIndex, x, y - 1, z))
				Num++;
			if(CheckPosHasCell(CellSpaceIndex, x + 1, y, z))
				Num++;
			if(CheckPosHasCell(CellSpaceIndex, x - 1, y, z))
				Num++;
			break;
		default:
			break;
	}
	
	return Num;
}

bool LifeGame3DWorld::CheckPosHasCell(uint8_t CellSpaceIndex, int x, int y, int z)
{
	return CellSpaceIndex >= 0 && CellSpaceIndex < 2 && x >=0 && x < SpaceLength && y >=0 && y < SpaceWidth && z >= 0 && z < SpaceHeight && CellSpace[CellSpaceIndex][x][y][z] == State;
}
