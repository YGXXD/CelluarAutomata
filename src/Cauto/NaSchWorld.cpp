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
		CellSpace[i] = std::vector<std::vector<CellInfo>>(RoadWidth, std::vector<CellInfo>(RoadLength));
	}

	CellArray = std::vector<std::vector<std::weak_ptr<Actor>>>(RoadWidth, std::vector<std::weak_ptr<Actor>>(RoadLength));

	// 随机初始化
	int XStart = -(RoadLength / 2);
	int YStart = -(RoadWidth / 2);
	for(int i = 0; i < RoadWidth; i++)
	{
		for(int j = 0; j < RoadLength; j++)
		{
			// 初始化五分之一概率生成细胞
			bool BirthProbability = (rand() % 5) == 0 && j < 150;
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
				if(j + CarSpeed < RoadLength)
				{
					CurrCellSpace[i][j + CarSpeed] = {true, CarSpeed};
					if(CellArray[i][j + CarSpeed].expired())
					{
						std::string Name = std::to_string(i) + "," + std::to_string(j + CarSpeed);
						CellArray[i][j + CarSpeed] = SpawnActor<Actor>(Name, Vector3(0, XStart + j, YStart + i));
					}
				}																											
				if(!CurrCellSpace[i][j].bHasCar)
				{
					if(!CellArray[i][j].expired())
					{
						DestroyActor(CellArray[i][j].lock());
					}	
				}
				LastCellSpace[i][j] = { };
			}	
		}

//		int num = 0;
//		for(int j = 0; j < RoadLength; ++j)
//		{
//			if(!CellArray[i][j].expired())	
//			{
//				num++;
//			}
//		}
//		std::cout << num << std::endl;
	}
}

int NaSchWorld::GetEmptyFront(int CellSpaceIndex, int x, int y)
{
	int Speed = 1;
	for(; Speed <= MaxSpeed; ++Speed)
	{
		if(y + Speed >= RoadLength)
			break;

		if(CellSpace[CellSpaceIndex][x][y + Speed].bHasCar)
		{
			return --Speed;
		}
	}
	
	return MaxSpeed;	
}
