#include "CrossRoadWorld.h"
#include <memory>

void CrossRoadWorld::BeginCreate()
{
	World::BeginCreate();
	
	InitCrossRoadWorld();
}

void CrossRoadWorld::WorldUpdate(float DeltaSeconds)
{
	World::WorldUpdate(DeltaSeconds);
	CurrSeconds += DeltaSeconds;
	if(CurrSeconds > TickInterval)
	{
		UpdateCrossRoadWorld();
		CurrSeconds = 0;
	}
}

void CrossRoadWorld::InitCrossRoadWorld()
{
	if(RoadWidth < 20)
		RoadWidth = 20;
	if(RoadHeight < 20)
		RoadHeight = 20;

	CellSpace = std::vector<std::vector<CrossRoadCellInfo>>(RoadWidth, std::vector<CrossRoadCellInfo>(RoadHeight, {0, 0, 0, 0}));
	//CellArray = std::vector<std::vector<std::weak_ptr<Actor>>>(RoadWidth, std::vector<std::weak_ptr<Actor>>(RoadHeight));

	CurrSeconds = 0;	

	// 初始化路网
	int XStart = -(RoadWidth / 2);
	int YStart = -(RoadHeight / 2);

	int WidthMid = RoadWidth >> 1;
	CrossWidthInterval[0] = WidthMid - 4;
   	CrossWidthInterval[1] =	WidthMid + 3;
	int HeightMid = RoadHeight >> 1;
	CrossHeightInterval[0] = HeightMid - 4;
    CrossHeightInterval[1] = HeightMid + 3;

	for(int i = 0; i < RoadWidth; ++i)
	{
		for(int j = 0; j < RoadHeight; ++j)
		{
			if((i < CrossWidthInterval[1] && i > CrossWidthInterval[0]) || (j < CrossHeightInterval[1] && j > CrossHeightInterval[0]))
			{
				CellSpace[i][j].CellType = 1;
				if(i <= CrossWidthInterval[0] || i >= CrossWidthInterval[1] || j <= CrossHeightInterval[0] || j >= CrossHeightInterval[1])
				{
					if(i < WidthMid && i > CrossWidthInterval[0])
					{
						CellSpace[i][j].Lane = 4;
						CellSpace[i][j].Turn = WidthMid - i;
					}
					else if(i >= WidthMid && i < CrossWidthInterval[1])
					{
						CellSpace[i][j].Lane = 3;
						CellSpace[i][j].Turn = i - WidthMid + 1;
					}
					else if(j < HeightMid && j > CrossHeightInterval[0])
					{
						CellSpace[i][j].Lane = 1;
						CellSpace[i][j].Turn = HeightMid - j;
					}
					else if(j >= HeightMid && j < CrossHeightInterval[1])
					{
						CellSpace[i][j].Lane = 2;
						CellSpace[i][j].Turn = j - HeightMid + 1;
					}
				}
			}
			else if(i == CrossWidthInterval[0] || i == CrossWidthInterval[1] || j == CrossHeightInterval[0] || j == CrossHeightInterval[1])
			{
				CellSpace[i][j].CellType = 2;

				std::string Name = std::to_string(i) + "," + std::to_string(j);
				auto Edge = SpawnActor<Actor>(Name, Vector3(0, XStart + i, YStart + j));
				Edge.lock()->SetRenderColor(Vector3(0, 0, 0));
				Edges.push_back(Edge);
			}
		}
	}

	// 初始化车辆
	for(int i = 0; i < RoadWidth; ++i)
	{
		for(int j = 0; j < RoadHeight; ++j)
		{
			if(CellSpace[i][j].CellType == 1 && CellSpace[i][j].Lane != 0)
			{
				if(rand() % 40 == 0)
				{
					CrossRoadCarInfo Car;
					std::string Name = std::to_string(i) + "," + std::to_string(j);
					Car.CellPtr = SpawnActor<Actor>(Name, Vector3(0, XStart + i, YStart + j));
					Car.Speed = 0;
					Car.CurrPos = {i, j};

					CarArray.push_back(std::move(Car));

					CellSpace[i][j].bHasCar = true;
				}
			}
		}
	}
}

void CrossRoadWorld::UpdateCrossRoadWorld()
{
	// 更新车辆
	int XStart = -(RoadWidth / 2);
	int YStart = -(RoadHeight / 2);

	auto TempCellSpace = CellSpace;	

	for(int i = 0; i < CarArray.size(); ++i)
	{
		auto& Car = CarArray[i];
		
		std::pair<int, int> FinalPos;
		if(CalcFinalPos(CellSpace[Car.CurrPos.first][Car.CurrPos.second].Lane, CellSpace[Car.CurrPos.first][Car.CurrPos.second].Turn, Car.CurrPos, FinalPos))
		{
			TempCellSpace[Car.CurrPos.first][Car.CurrPos.second].bHasCar = false; 
			TempCellSpace[FinalPos.first][FinalPos.second].bHasCar = true;

			if(IsInCrossCenter(FinalPos.first, FinalPos.second))
			{
				TempCellSpace[FinalPos.first][FinalPos.second].Turn = CellSpace[Car.CurrPos.first][Car.CurrPos.second].Turn;
				TempCellSpace[FinalPos.first][FinalPos.second].Lane = CellSpace[Car.CurrPos.first][Car.CurrPos.second].Lane;
			}

			if(IsInCrossCenter(Car.CurrPos.first, Car.CurrPos.second))
			{
				TempCellSpace[Car.CurrPos.first][Car.CurrPos.second].Turn = 0;
				TempCellSpace[Car.CurrPos.first][Car.CurrPos.second].Lane = 0;
			}
			
			Car.Speed = 0;
			Car.CurrPos = FinalPos;
			// std::cout << (int)TempCellSpace[Car.CurrPos.first][Car.CurrPos.second].Lane << (int)TempCellSpace[Car.CurrPos.first][Car.CurrPos.second].Turn << std::endl;
			Car.CellPtr.lock()->SetActorLocation(Vector3(0, XStart + Car.CurrPos.first, YStart + Car.CurrPos.second));
		}
	}
	
	CellSpace = TempCellSpace;

	/*
	int num = 0;
	for(int i = 0; i < RoadWidth; ++i)
	{
		for(int j = 0; j < RoadHeight; ++j)
		{
			if(CellSpace[i][j].bHasCar)
			{
				num++;
			}
		}
	}

	std::cout << num << "," << CarArray.size() << std::endl;
	*/
}

std::pair<int, int> CrossRoadWorld::CalcRelativePosition(int Lane, int x, int y, int xOffset, int yOffset)
{
	std::pair<int, int> PosOffset = { };
	switch(Lane)
	{
		case 1:
			PosOffset.first = yOffset;
			PosOffset.second = -xOffset;
			break;
		case 2:
			PosOffset.first = -yOffset;
			PosOffset.second = xOffset;
			break;
		case 3:
			PosOffset.first = xOffset;
			PosOffset.second = yOffset;
			break;
		case 4:
			PosOffset.first = -xOffset;
			PosOffset.second = -yOffset;
			break;
		default:
			break;
	}

	std::pair<int, int> OutPos = {x + PosOffset.first, y + PosOffset.second};

	while(OutPos.first < 0)
	{
		OutPos.first += RoadWidth;
	}
	while(OutPos.second < 0)
	{
		OutPos.second += RoadHeight;
	}

	return {OutPos.first % RoadWidth, OutPos.second % RoadHeight};
}

bool CrossRoadWorld::CalcFinalPos(int Lane, int Turn, const std::pair<int, int>& InCurrPos, std::pair<int, int>& OutFinalPos)
{
	OutFinalPos = InCurrPos;

	std::pair<int, int> Pos1;
	if(InCurrPos.first >= CrossWidthInterval[0] + 3 && InCurrPos.second >= CrossHeightInterval[0] + 3 && InCurrPos.second <= CrossHeightInterval[0] + 4 && InCurrPos.first <= CrossWidthInterval[0] + 4)
	{
		// std::cout << 3 << std::endl;
		// 都是左转
		Pos1 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, -1, 1);
		
		const auto& Cell1 = CellSpace[Pos1.first][Pos1.second];
		if(!Cell1.bHasCar) // 没有车
		{
			std::pair<int, int> Pos2 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, -1, 0);
			
			const auto& Cell2 = CellSpace[Pos2.first][Pos2.second];
			if(!Cell2.bHasCar || Cell2.Turn == 2) // 没有车或者直行
			{
				std::pair<int, int> Pos3 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, -1, 2);
				const auto& Cell3 = CellSpace[Pos3.first][Pos3.second];
				if(!Cell3.bHasCar || Math::Abs(Cell3.Lane - Lane) != 1)
				{
					OutFinalPos = Pos1;					
					return 1;
				}		
			}
		}
	}
	else if(InCurrPos.first >= CrossWidthInterval[0] + 2 && InCurrPos.second >= CrossHeightInterval[0] + 2 && InCurrPos.first <= CrossWidthInterval[0] + 5 && InCurrPos.second <= CrossHeightInterval[0] + 5)
	{
		// std::cout << 2 << std::endl;
		switch (Turn)
		{
			// 左转
			case 1:
				{
					Pos1 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, -1, 1);
					// 判断是左转进入点还是驶出点
					if(Pos1.first >= CrossWidthInterval[0] + 3 && Pos1.first <= CrossWidthInterval[0] + 4 && Pos1.second >= CrossHeightInterval[0] + 3 && Pos1.second <= CrossHeightInterval[0] + 4)
					{
						if(!CellSpace[Pos1.first][Pos1.second].bHasCar)
						{
							OutFinalPos = Pos1;
							return 1;
						}
					}
					else 
					{
						Pos1 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, -1, 0);
						if(!CellSpace[Pos1.first][Pos1.second].bHasCar)
						{
							OutFinalPos = Pos1;
							return 1;
						}
					}
				}
				break;
			
			// 直行
			case 2:
				{
					Pos1 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, 0, 1);
					if(!CellSpace[Pos1.first][Pos1.second].bHasCar)
					{
						OutFinalPos = Pos1;
						return 1;		
					}
				}
				break;

			default:
				break;
		}
	}
	else if(InCurrPos.first >= CrossWidthInterval[0] + 1 && InCurrPos.first <= CrossWidthInterval[0] + 6 && InCurrPos.second >= CrossHeightInterval[0] + 1 && InCurrPos.second <= CrossHeightInterval[0] + 6)
	{
		// std::cout << 1 << std::endl;
		switch(Turn)
		{
			// 左转
			case 1:
				{
					Pos1 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, 0, 1);
					if(Pos1.first >= CrossWidthInterval[0] + 2 && Pos1.first <= CrossWidthInterval[0] + 5 && Pos1.second >= CrossHeightInterval[0] + 2 && Pos1.second <= CrossHeightInterval[0] + 5)
					{
						if(!CellSpace[Pos1.first][Pos1.second].bHasCar)
						{
							std::pair<int, int> Pos2 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, 0, 2);						
							if(!CellSpace[Pos2.first][Pos2.second].bHasCar)
							{
								std::pair<int, int> Pos3 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, -1, 1);
								const auto& Cell3 = CellSpace[Pos3.first][Pos3.second];
								if(!Cell3.bHasCar || Math::Abs(Cell3.Lane - Lane) == 1)
								{
									OutFinalPos = Pos1;
									return 1;
								}
							}				
						}
					}
					else
					{
						Pos1 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, -1, 0);
						if(!CellSpace[Pos1.first][Pos1.second].bHasCar)
						{
							OutFinalPos = Pos1;
							return 1;
						}
					}
				}
				break;

			// 直行
			case 2:	
				{
					Pos1 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, 0, 1);
					if(Pos1.first >= CrossWidthInterval[0] + 2 && Pos1.first <= CrossWidthInterval[0] + 5 && Pos1.second >= CrossHeightInterval[0] + 2 && Pos1.second <= CrossHeightInterval[0] + 5)
					{
						if(!CellSpace[Pos1.first][Pos1.second].bHasCar)
						{
							std::pair<int, int> Pos2 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, -1, 1);
							const auto& Cell2 = CellSpace[Pos2.first][Pos2.second];
							if(!Cell2.bHasCar || Cell2.Turn == 1)
							{
								std::pair<int, int> Pos3 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, -2, 1);
								const auto& Cell3 = CellSpace[Pos3.first][Pos3.second];
								if(!Cell3.bHasCar || Cell3.Turn == 1)
								{
									std::pair<int, int> Pos4 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, -3, 1);
									const auto& Cell4 = CellSpace[Pos4.first][Pos4.second];
									if(!Cell4.bHasCar || Math::Abs(Cell4.Lane - Lane) == 1)
									{
										OutFinalPos = Pos1;
										return 1;
									}
								}
							}
						}
					}
					else
					{
						Pos1 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, 0, 1);
						if(!CellSpace[Pos1.first][Pos1.second].bHasCar)
						{
							OutFinalPos = Pos1;
							return 1;
						}
					}
				}
				break;

			// 右转
			case 3:
				{
					Pos1 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, 1, 0);
					if(!CellSpace[Pos1.first][Pos1.second].bHasCar)
					{
						OutFinalPos = Pos1;
						return 1;
					}
				}
				break;
		}
	}
	else
	{
		// std::cout << 0 << std::endl;
		{
			Pos1 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, 0, 1);
			if(!CellSpace[Pos1.first][Pos1.second].bHasCar)
			{
				OutFinalPos = Pos1;
				return 1;
			}
		}
	}

	return 0;
}

bool CrossRoadWorld::IsInCrossCenter(int x, int y)
{
	return x > CrossWidthInterval[0] && x < CrossWidthInterval[1] && y > CrossHeightInterval[0] && y < CrossHeightInterval[1];
}
