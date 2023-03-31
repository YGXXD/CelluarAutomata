#include "BMLWorld.h"
#include <iostream>

void BMLWorld::BeginCreate()
{
	World::BeginCreate();
	InitBMLWorld();	
}

void BMLWorld::WorldUpdate(float DeltaSeconds)
{
	World::WorldUpdate(DeltaSeconds);
	CurrSeconds += DeltaSeconds;
	if(CurrSeconds > TickInterval)
	{
		UpdateBMLWorld();
		CurrSeconds = 0;
	}
}

void BMLWorld::InitBMLWorld()
{
	CurrCellSpaceIndex = 0;
	CurrDirection = 0;
	for(int i = 0; i < 2; i++)
	{
		CellSpace[i] = std::vector<std::vector<EBMLDirection>>(RoadWidth, std::vector<EBMLDirection>(RoadHeight, EBMLDirection::Max));
	}
	CellArray = std::vector<std::vector<std::weak_ptr<Actor>>>(RoadWidth, std::vector<std::weak_ptr<Actor>>(RoadHeight));

	int XStart = -(RoadWidth / 2);
	int YStart = -(RoadHeight / 2);
	for(int i = 0; i < RoadWidth; i++)
	{
		for(int j = 0; j < RoadHeight; j++)
		{
			// 初始化概率生成细胞
			bool BirthProbability = (rand() % 8) == 0;
			if(BirthProbability)
			{
				uint8_t Direction = rand() % static_cast<uint8_t>(EBMLDirection::Max);
				CellSpace[CurrCellSpaceIndex][i][j] = static_cast<EBMLDirection>(Direction);

				std::string Name = std::to_string(i) + "," + std::to_string(j);
				CellArray[i][j] = SpawnActor<Actor>(Name, Vector3(0, XStart + i, YStart + j));
				CellArray[i][j].lock()->SetRenderColor(ColorMap[Direction]);
			}
		}
	}
}

void BMLWorld::UpdateBMLWorld()
{
	int XStart = -(RoadWidth / 2);
	int YStart = -(RoadHeight / 2);

	uint8_t LastCellSpaceIndex = CurrCellSpaceIndex;
	CurrCellSpaceIndex = ++CurrCellSpaceIndex % 2;
	auto& LastCellSpace = CellSpace[LastCellSpaceIndex];
	auto& CurrCellSpace = CellSpace[CurrCellSpaceIndex];

	std::pair<int, int> V = {0, 0};
	switch(static_cast<EBMLDirection>(CurrDirection))
  	{
		case EBMLDirection::Right:
			V.first = 1;
			break;
		case EBMLDirection::Left:
			V.first = -1;
			break;
		case EBMLDirection::Up:
			V.second = 1;
			break;
		case EBMLDirection::Down:
			V.second = -1;
			break;
		default:
			break;
	}
	
	for(int i = 0; i < RoadWidth; ++i)
	{
		for(int j = 0; j < RoadHeight; ++j)
		{
			if(LastCellSpace[i][j] < EBMLDirection::Max)
			{
				CurrCellSpace[i][j] = LastCellSpace[i][j];

				if(static_cast<uint8_t>(LastCellSpace[i][j]) == CurrDirection)
				{	
					int NewI = i + V.first;
					int NewJ = j + V.second;
	
					if(!CheckPosHasCell(LastCellSpaceIndex, NewI, NewJ))
					{
						CurrCellSpace[NewI][NewJ] = LastCellSpace[i][j];
						CurrCellSpace[i][j] = EBMLDirection::Max;
	
						if(CellArray[NewI][NewJ].expired())
						{
							std::string Name = std::to_string(NewI) + "," + std::to_string(NewJ);
							CellArray[NewI][NewJ] = SpawnActor<Actor>(Name, Vector3(0, XStart + NewI, YStart + NewJ));
							CellArray[NewI][NewJ].lock()->SetRenderColor(ColorMap[CurrDirection]);
						}
						if(!CellArray[i][j].expired())
							DestroyActor(CellArray[i][j].lock());	
					}
				}
			}
		}
	}

	for(int i = 0; i < RoadWidth; ++i)
	{
		for(int j = 0; j < RoadHeight; ++j)
		{
			LastCellSpace[i][j] = EBMLDirection::Max;
		}
	}

	CurrDirection = (CurrDirection + 1) % static_cast<uint8_t>(EBMLDirection::Max);
}

bool BMLWorld::CheckPosHasCell(uint8_t CellSpaceIndex, int& x, int& y)
{
	CellSpaceIndex %= 2;
	while(x < 0)
	{
		x += RoadWidth;
	}
	while(y < 0)
	{
		y += RoadHeight;
	}
	x %= RoadWidth;
	y %= RoadHeight;

	return CellSpace[CellSpaceIndex][x][y] < EBMLDirection::Max;
}
