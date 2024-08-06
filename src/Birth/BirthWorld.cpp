#include "BirthWorld.h"
#include "BirthCamera.h"
#include "../Math/MathLibrary.h"
#include <algorithm>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

BirthWorld::BirthWorld() : World()
{

}

BirthWorld::~BirthWorld()
{

}

void BirthWorld::BeginCreate()
{
	World::BeginCreate();
	SpawnActor<BirthCamera>("BirthCamera");
	GameOfLifeInit();		
}

void BirthWorld::WorldUpdate(float DeltaSeconds)
{
	World::WorldUpdate(DeltaSeconds);
	CurrSeconds += DeltaSeconds;
	if(CurrSeconds >= TickInterval)
	{
		// GameOfLifeUpdate();
		TickInterval = Math::Clamp(0.02f, TickInterval, TickInterval * 0.5f);
		CurrSeconds = 0.f;
	}
	
}

void BirthWorld::BeginDestroy()
{
	World::BeginDestroy();
}

void BirthWorld::GameOfLifeInit()
{
	const std::string ImagePath = IMAGE_PATH;
    int ImageWidth, ImageHeight, Channels;
    unsigned char* data = stbi_load((ImagePath + "number.png").c_str(), &ImageWidth, &ImageHeight, &Channels, 0);
	if (data == nullptr)
		std::cout << "数字加载失败" << std::endl;

	std::sort(Survival.begin(), Survival.end());
	std::sort(Birth.begin(), Birth.end());

	CurrCellSpaceIndex = 0;
	for(int i = 0; i < 2; i++)
	{
		CellSpace[i] = std::vector<std::vector<bool>>(SpaceWidth, std::vector<bool>(SpaceHeight, false));
	}
	CellArray = std::vector<std::vector<std::weak_ptr<BirthActor>>>(SpaceWidth, std::vector<std::weak_ptr<BirthActor>>(SpaceHeight));
	
	int XStart = -(SpaceWidth / 2);
	int YStart = -(SpaceHeight / 2);
	for(int i = 0; i < SpaceWidth; i++)
	{
		for(int j = 0; j < SpaceHeight; j++)
		{
			// 初始化五分之一概率生成细胞
			float U = static_cast<float>(i) / static_cast<float>(SpaceWidth);
			float V = static_cast<float>(SpaceHeight - 1 - j) / static_cast<float>(SpaceHeight);

			int ImageX = static_cast<int>(U * ImageWidth);
			int ImageY = static_cast<int>(V * ImageHeight);
			uint8_t Color = data[Channels * (ImageX + ImageY * ImageWidth)];

			bool BirthProbability = Color < 250;
			if(BirthProbability)
			{
				CellSpace[CurrCellSpaceIndex][i][j] = true;
				std::string Name = std::to_string(i) + "," + std::to_string(j);
				Vector3 Location = Math::RandBoxVector(0, SpaceWidth * 0.5f, SpaceHeight * 0.5f);
				CellArray[i][j] = SpawnActor<BirthActor>(Name, Location);
				CellArray[i][j].lock()->StartLocation = Location;
				CellArray[i][j].lock()->MoveToLocation = Vector3(0, XStart + i, YStart + j);
				CellArray[i][j].lock()->SetRenderColor(CellColor);
				CellArray[i][j].lock()->SetIsUpdate(true);
			}
		}
	}
	std::cout << std::endl;
	stbi_image_free(data);
}

void BirthWorld::GameOfLifeUpdate()
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
					CellArray[i][j] = SpawnActor<BirthActor>(Name, Vector3(0, XStart + i, YStart + j));
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

uint8_t BirthWorld::FindCellNearNum(uint8_t CellSpaceIndex, int x, int y)
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

bool BirthWorld::CheckPosHasCell(uint8_t CellSpaceIndex, int x, int y)
{
	return CellSpaceIndex >=0 && CellSpaceIndex < 2 && x >=0 && x < SpaceWidth && y >=0 && y < SpaceHeight && CellSpace[CellSpaceIndex][x][y];
}
