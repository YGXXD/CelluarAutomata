#include "RoadInstWorld.h"
#include <memory>
#include <iostream>

void RoadInstWorld::BeginCreate()
{
	World::BeginCreate();
	
	InitRoadInstWorld();
}

void RoadInstWorld::WorldUpdate(float DeltaSeconds)
{
	World::WorldUpdate(DeltaSeconds);
	CurrSeconds += DeltaSeconds;
	if(CurrSeconds > TickInterval)
	{
		UpdateRoadInstWorld();
		CurrSeconds = 0;
	}
}

void RoadInstWorld::InitRoadInstWorld()
{
	if(RoadWidth < 24)
		RoadWidth = 24;
	if(RoadHeight < 24)
		RoadHeight = 24;

	CellSpace = std::vector<std::vector<RoadInstCellInfo>>(RoadWidth, std::vector<RoadInstCellInfo>(RoadHeight, {0, 0, 0, 0}));
	//CellArray = std::vector<std::vector<std::weak_ptr<Actor>>>(RoadWidth, std::vector<std::weak_ptr<Actor>>(RoadHeight));

	CurrSeconds = 0;	

	// 初始化路网
	int XStart = -(RoadWidth / 2);
	int YStart = -(RoadHeight / 2);

	int RoadWidthMid = RoadWidth >> 1;
	CrossWidthInterval[0] = RoadWidthMid - 4;
   	CrossWidthInterval[1] =	RoadWidthMid + 3;
	int CrossHeightMid = RoadHeight * 2 / 3;
	CrossHeightInterval[0] = CrossHeightMid - 4;
    CrossHeightInterval[1] = CrossHeightMid + 3;

	int ForkHeightMid = RoadHeight / 3;
	ForkHeightInterval[0] = ForkHeightMid - 2;
	ForkHeightInterval[1] = ForkHeightMid + 1;

	for(int i = 0; i < RoadWidth; ++i)
	{
		for(int j = 0; j < RoadHeight; ++j)
		{
			if((i < CrossWidthInterval[1] && i > CrossWidthInterval[0]) || (j < CrossHeightInterval[1] && j > CrossHeightInterval[0]) || (j < ForkHeightInterval[1] && j > ForkHeightInterval[0]))
			{
				CellSpace[i][j].CellType = 1;

				if(IsInCrossForkRoad(i, j))
				{
					continue;
				}
				else if(i < RoadWidthMid && i > CrossWidthInterval[0])
				{
					CellSpace[i][j].Lane = 4;
					CellSpace[i][j].Turn = RoadWidthMid - i;
				}
				else if(i >= RoadWidthMid && i < CrossWidthInterval[1])
				{
					
					CellSpace[i][j].Lane = 3;
					CellSpace[i][j].Turn = i - RoadWidthMid + 1;
				}
				else if(j < CrossHeightMid && j > CrossHeightInterval[0])
				{
					CellSpace[i][j].Lane = 1;
					CellSpace[i][j].Turn = CrossHeightMid - j;
				}
				else if(j >= CrossHeightMid && j < CrossHeightInterval[1])
				{
					CellSpace[i][j].Lane = 2;
					CellSpace[i][j].Turn = j - CrossHeightMid + 1;
				}
				else if(j >= ForkHeightMid && j < ForkHeightInterval[1])
				{
					CellSpace[i][j].Lane = 2;
					CellSpace[i][j].Turn = 3;
				}
				else if(j < ForkHeightMid && j > ForkHeightInterval[0])
				{
					CellSpace[i][j].Lane = 1;
					CellSpace[i][j].Turn = 3;
				}

				// 添加交通灯
				if((i == CrossWidthInterval[0] || i == CrossWidthInterval[1]) && j > CrossHeightInterval[0] + 1 && j < CrossHeightInterval[1] - 1)
				{
					std::string Name = "Light:" + std::to_string(i) + "," + std::to_string(j);
					auto Light = SpawnActor<Actor>(Name, Vector3(-5, XStart + i, YStart + j));
					WidthLight.Lights.push_back(Light);
				}	
				else if((j == CrossHeightInterval[0] || j == CrossHeightInterval[1]) && i > CrossWidthInterval[0] + 1 && i < CrossWidthInterval[1] - 1)
				{
					std::string Name = "Light:" + std::to_string(i) + "," + std::to_string(j);
					auto Light = SpawnActor<Actor>(Name, Vector3(-5, XStart + i, YStart + j));
					HeightLight.Lights.push_back(Light);
				}	
			}
			else if(i == CrossWidthInterval[0] || i == CrossWidthInterval[1] || j == CrossHeightInterval[0] || j == CrossHeightInterval[1] || j == ForkHeightInterval[0] || j == ForkHeightInterval[1])
			{
				CellSpace[i][j].CellType = 2;

				std::string Name = std::to_string(i) + "," + std::to_string(j);
				auto Edge = SpawnActor<Actor>(Name, Vector3(0, XStart + i, YStart + j));
				Edge.lock()->SetRenderColor(Vector3(0, 0, 0));
				Edges.push_back(Edge);
			}
		}
	}

	// 初始化交通灯
	SetTrafficLightColor(WidthLight, 0);
	SetTrafficLightColor(HeightLight, 1);

	// 初始化车辆
	for(int i = 0; i < RoadWidth; ++i)
	{
		for(int j = 0; j < RoadHeight; ++j)
		{
			if(CellSpace[i][j].CellType == 1 && CellSpace[i][j].Lane != 0)
			{
				//std::string Name = std::to_string(i) + "," + std::to_string(j);
				//auto CellPtr = SpawnActor<Actor>(Name, Vector3(0, XStart + i, YStart + j));
				//CellPtr.lock()->SetRenderColor(Vector3(CellSpace[i][j].Turn/3.0,0,0));
				//a.push_back(CellPtr);

				if(rand() % 40 == 0)
				{
					RoadInstCarInfo Car;
					std::string Name = std::to_string(i) + "," + std::to_string(j);
					Car.CellPtr = SpawnActor<Actor>(Name, Vector3(0, XStart + i, YStart + j));
					Car.CellPtr.lock()->SetRenderColor(Vector3(0.5, 0.5, (float)rand() / RAND_MAX));
					Car.Speed = 0;
					Car.CurrPos = {i, j};

					CarArray.push_back(std::move(Car));

					CellSpace[i][j].bHasCar = true;
				}
			}
		}
	}
}

void RoadInstWorld::UpdateRoadInstWorld()
{
	
	// 更新交通灯
	++CurrTime;
	if(WidthLight.Color == 1)
	{
		if(CurrTime >= GreenTime + DelayTime)
		{
			SetTrafficLightColor(WidthLight, 0);
			SetTrafficLightColor(HeightLight, 1);
			CurrTime = 0;	
		}
		else if(CurrTime >= GreenTime && HeightLight.Color == 0)
		{
			SetTrafficLightColor(HeightLight, 2);
		}
	}
	else
	{
		if(CurrTime >= GreenTime + DelayTime)
		{
			SetTrafficLightColor(WidthLight, 1);
			SetTrafficLightColor(HeightLight, 0);
			CurrTime = 0;	
		}
		else if(CurrTime >= GreenTime && WidthLight.Color == 0)
		{
			SetTrafficLightColor(WidthLight, 2);
		}
	}

	// 更新车辆
	int XStart = -(RoadWidth / 2);
	int YStart = -(RoadHeight / 2);

	auto TempCellSpace = CellSpace;	

	for(int i = 0; i < CarArray.size(); ++i)
	{
		auto& Car = CarArray[i];
		
		bool bMove = false;
		std::pair<int, int> FinalPos;
		int FinalSpeed = 0;
		if(!IsInCrossForkRoad(Car.CurrPos.first, Car.CurrPos.second))
		{
			FinalSpeed = CalcFinalPosInStraight(CellSpace[Car.CurrPos.first][Car.CurrPos.second].Lane, CellSpace[Car.CurrPos.first][Car.CurrPos.second].Turn, Car.Speed, Car.CurrPos, FinalPos);
			if(FinalSpeed > 0)
			{
				TempCellSpace[Car.CurrPos.first][Car.CurrPos.second].bHasCar = false; 
				
				TempCellSpace[FinalPos.first][FinalPos.second].bHasCar = true;
				
				if(IsInCrossForkRoad(FinalPos.first, FinalPos.second))
				{
					TempCellSpace[FinalPos.first][FinalPos.second].Turn = CellSpace[Car.CurrPos.first][Car.CurrPos.second].Turn;
					TempCellSpace[FinalPos.first][FinalPos.second].Lane = CellSpace[Car.CurrPos.first][Car.CurrPos.second].Lane;
				}

				bMove = true;
			}
		}
		else
		{
			if(CalcFinalPosInCrossFork(CellSpace[Car.CurrPos.first][Car.CurrPos.second].Lane, CellSpace[Car.CurrPos.first][Car.CurrPos.second].Turn, Car.CurrPos, FinalPos))
			{
				FinalSpeed = 1;

				TempCellSpace[Car.CurrPos.first][Car.CurrPos.second].bHasCar = false; 
				TempCellSpace[Car.CurrPos.first][Car.CurrPos.second].Turn = 0;
				TempCellSpace[Car.CurrPos.first][Car.CurrPos.second].Lane = 0;
				
				TempCellSpace[FinalPos.first][FinalPos.second].bHasCar = true;

				if(IsInCrossForkRoad(FinalPos.first, FinalPos.second))
				{
					TempCellSpace[FinalPos.first][FinalPos.second].Turn = CellSpace[Car.CurrPos.first][Car.CurrPos.second].Turn;
					TempCellSpace[FinalPos.first][FinalPos.second].Lane = CellSpace[Car.CurrPos.first][Car.CurrPos.second].Lane;
				}
				
				bMove = true;
			}
		}

		Car.Speed = FinalSpeed;

		if(bMove)
		{
			Car.CurrPos = FinalPos;
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

std::pair<int, int> RoadInstWorld::CalcRelativePosition(int Lane, int x, int y, int xOffset, int yOffset)
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

bool RoadInstWorld::CalcFinalPosInCrossFork(int Lane, int Turn, const std::pair<int, int>& InCurrPos, std::pair<int, int>& OutFinalPos)
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
	else if((InCurrPos.first == CrossWidthInterval[0] + 1 || InCurrPos.first == CrossWidthInterval[1] - 1) && InCurrPos.second > ForkHeightInterval[0] && InCurrPos.second < ForkHeightInterval[1])
   	{
		// 右转
		Pos1 = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, 1, 0);
		if(!CellSpace[Pos1.first][Pos1.second].bHasCar)
		{
			OutFinalPos = Pos1;
			return 1;
		}	
	}

	return 0;
}

int RoadInstWorld::CalcFinalPosInStraight(int Lane, int Turn, int CurrSpeed, const std::pair<int,int>& InCurrPos, std::pair<int,int>& OutFinalPos)
{
	// std::cout << 0 << std::endl;

	auto Pos = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, 0, 1);
	
	if(CellSpace[Pos.first][Pos.second].bHasCar)
	{
		OutFinalPos = InCurrPos;
		return 0;
	}

	if(IsInCrossForkRoad(Pos.first, Pos.second)) //是否进入十字路口或转弯
	{
		if(CanCrossTrafficLightRoad(Lane) || Turn == 3)
		{
			OutFinalPos = Pos;
			return 1;
		}
		else
		{
			OutFinalPos = InCurrPos;
			return 0;
		}
	}

	// 加速减速慢随机
	int Empty = 1;
	while(Empty < MaxSpeed)
	{
		Pos = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, 0, Empty + 1);
		if(IsInCrossForkRoad(Pos.first, Pos.second) || CellSpace[Pos.first][Pos.second].bHasCar)
			break;
		++Empty;
	}

	int FinalSpeed = CalcFinalSpeed(CurrSpeed, Empty);
	if(FinalSpeed > 0)
		OutFinalPos = CalcRelativePosition(Lane, InCurrPos.first, InCurrPos.second, 0, FinalSpeed);
	else
		OutFinalPos = InCurrPos;

	return FinalSpeed;
}

int RoadInstWorld::CalcFinalSpeed(int CurrSpeed, int EmptyOfFront)
{
	int CarSpeed = CurrSpeed;
	CarSpeed = Math::Min(CarSpeed + 1, MaxSpeed); //加速
	CarSpeed = Math::Min(CarSpeed, EmptyOfFront); //减速
	if((float)rand() / RAND_MAX < SlowDown)
		CarSpeed = Math::Max(CarSpeed - 1, 0); //慢随机化
	
	return CarSpeed;
}

bool RoadInstWorld::IsInCrossForkRoad(int x, int y)
{
	if((x == CrossWidthInterval[0] + 1 || x == CrossWidthInterval[1] - 1) && y > ForkHeightInterval[0] && y < ForkHeightInterval[1])
		return true;
	return x > CrossWidthInterval[0] && x < CrossWidthInterval[1] && y > CrossHeightInterval[0] && y < CrossHeightInterval[1];
}

void RoadInstWorld::SetTrafficLightColor(TrafficLightInfo& TrafficLight, int Color)
{
	Color %= 3;
	Vector3 C;
	switch (Color)
	{
		case 0:
			C = Vector3(0, 1, 0); 
			break;
		case 1:
			C = Vector3(1, 0, 0);
			break;
		case 2:
			C = Vector3(1, 1, 0);
			break;
		default:
			return;
	}

	TrafficLight.Color = Color;
	for(const auto& it : TrafficLight.Lights)
	{
		it.lock()->SetRenderColor(C);		
	}
}

bool RoadInstWorld::CanCrossTrafficLightRoad(int Lane)
{
	switch (Lane)
   	{
		case 1:
		case 2:
			return WidthLight.Color == 0;
		case 3:
		case 4:
			return HeightLight.Color == 0;
		default:
			return false;
	}
}
