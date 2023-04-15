#include "NaSchWorld.h"
#include "../Math/MathLibrary.h"
#include <iostream>

void NaSchWorld::BeginCreate()
{
	World::BeginCreate();
	InitNaSchWorld();	
}

void NaSchWorld::WorldUpdate(float DeltaSeconds)
{
	World::WorldUpdate(DeltaSeconds);
	CurrSeconds += DeltaSeconds;
	if(CurrSeconds > TickInterval)
	{
		UpdateNaSchWorld();
		CurrSeconds = 0;
	}
}

void NaSchWorld::InitNaSchWorld()
{
	CurrCellSpaceIndex = 0;
	for(int i = 0; i < 2; ++i)
	{
		CellSpace[i] = std::vector<std::vector<NaSchCellInfo>>(RoadWidth, std::vector<NaSchCellInfo>(RoadLength));
	}

	CellArray = std::vector<std::vector<std::weak_ptr<Actor>>>(RoadWidth, std::vector<std::weak_ptr<Actor>>(RoadLength));

	// 随机初始化
	int XStart = -(RoadLength / 2);
	int YStart = -(RoadWidth / 2);
	for(int i = 0; i < RoadWidth; i++)
	{
		for(int j = 0; j < RoadLength; j++)
		{
			// 初始化概率生成细胞
			bool BirthProbability = (rand() % 4) == 0;
			if(BirthProbability)
			{
				CellSpace[CurrCellSpaceIndex][i][j] = {true, 0};
				std::string Name = std::to_string(i) + "," + std::to_string(j);
				CellArray[i][j] = SpawnActor<Actor>(Name, Vector3(0, XStart + j, YStart + i));
			}
		}
	}
}

void NaSchWorld::UpdateNaSchWorld()
{
	int XStart = -(RoadLength / 2);
	int YStart = -(RoadWidth / 2);

	uint8_t LastCellSpaceIndex = CurrCellSpaceIndex;
	CurrCellSpaceIndex = ++CurrCellSpaceIndex % 2;
	auto& LastCellSpace = CellSpace[LastCellSpaceIndex];
	auto& CurrCellSpace = CellSpace[CurrCellSpaceIndex];

	for(int i = 0; i < RoadWidth; ++i)
	{
		for(int j = 0; j < RoadLength; ++j)
		{
			if(LastCellSpace[i][j].bHasCar)
			{
				int CarSpeed = LastCellSpace[i][j].Speed;
				CarSpeed = Math::Max(CarSpeed + 1, MaxSpeed); //加速
				CarSpeed = Math::Min(CarSpeed, GetEmptyFront(LastCellSpaceIndex, i, j)); //减速
				if((rand() % 1024) / 1024.f < SlowDown)
					CarSpeed = Math::Max(CarSpeed - 1, 0); //慢随机化
				
				// 更新细胞
				int NewJ = (j + CarSpeed) % RoadLength;
				CurrCellSpace[i][NewJ] = {true, CarSpeed};
				if(CellArray[i][NewJ].expired())
				{
					std::string Name = std::to_string(i) + "," + std::to_string(NewJ);
					CellArray[i][NewJ] = SpawnActor<Actor>(Name, Vector3(0, XStart + NewJ, YStart + i));
				}

				if(!CurrCellSpace[i][j].bHasCar)
				{
					if(!CellArray[i][j].expired())
					{
						DestroyActor(CellArray[i][j].lock());
					}	
				}
			}	
		}
	}
	for(int i = 0; i < RoadWidth; ++i)
	{
		for(int j = 0; j < RoadLength; ++j)
		{
			LastCellSpace[i][j] = { };
		}
	}

/*
	int num = 0;
	for(int i = 0; i < RoadWidth; ++i)
	{
		for(int j = 0; j < RoadLength; ++j)
		{
			if(CurrCellSpace[i][j].bHasCar)
				num++;
		}
	}

	std::cout << num << std::endl;
*/
}

int NaSchWorld::GetEmptyFront(uint8_t CellSpaceIndex, int x, int y)
{
	if(x < 0 || y < 0 || x >= RoadWidth || y >= RoadLength)
		return 0;

	CellSpaceIndex %= 2;
	int Speed = 1;
	for(; Speed <= MaxSpeed; ++Speed)
	{
		if(CellSpace[CellSpaceIndex][x][(y + Speed) % RoadLength].bHasCar)
		{
			return --Speed;
		}
	}
	
	return MaxSpeed;	
}
