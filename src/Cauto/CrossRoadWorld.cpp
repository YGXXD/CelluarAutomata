#include "CrossRoadWorld.h"
#include "../Math/MathLibrary.h"

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
		RoadWidth = 16;
	if(RoadHeight < 20)
		RoadHeight = 16;

	GridSpace = std::vector<std::vector<CrossRoadGrid>>(RoadWidth, std::vector<CrossRoadGrid>(RoadHeight, {false, CrossRoadGridType::Max, CrossRoadDirection::Max}));
	//CellArray = std::vector<std::vector<std::weak_ptr<Actor>>>(RoadWidth, std::vector<std::weak_ptr<Actor>>(RoadHeight));

	CurrTime = 0;	

	// 初始化路网
	int XStart = -(RoadWidth / 2);
	int YStart = -(RoadHeight / 2);

	int WidthMid = RoadWidth >> 1;
	CrossWidthInterval[0] = WidthMid - 5;
   	CrossWidthInterval[1] =	WidthMid + 4;
	int HeightMid = RoadHeight >> 1;
	CrossHeightInterval[0] = HeightMid - 5;
    CrossHeightInterval[1] = HeightMid + 4;

	for(int i = 0; i < RoadWidth; ++i)
	{
		for(int j = 0; j < RoadHeight; ++j)
		{
			if((i < CrossWidthInterval[1] && i > CrossWidthInterval[0]) || (j < CrossHeightInterval[1] && j > CrossHeightInterval[0]))
			{
				GridSpace[i][j].GridType = CrossRoadGridType::Road;
				if(i <= CrossWidthInterval[0] || i >= CrossWidthInterval[1] || j <= CrossHeightInterval[0] || j >= CrossHeightInterval[1])
				{
					if(i < WidthMid && i > CrossWidthInterval[0])
					{
						GridSpace[i][j].GridDirection = CrossRoadDirection::Down;
					}
					else if(i >= WidthMid && i < CrossWidthInterval[1])
					{
						GridSpace[i][j].GridDirection = CrossRoadDirection::Up;
					}
					else if(j < HeightMid && j > CrossHeightInterval[0])
					{
						GridSpace[i][j].GridDirection = CrossRoadDirection::Right;
					}
					else if(j >= HeightMid && j < CrossHeightInterval[1])
					{
						GridSpace[i][j].GridDirection = CrossRoadDirection::Left;
					}
				}

				if(i == CrossWidthInterval[0] || i == CrossWidthInterval[1])
				{
					std::string Name = "Light:" + std::to_string(i) + "," + std::to_string(j);
					auto Light = SpawnActor<Actor>(Name, Vector3(-5, XStart + i, YStart + j));
					WidthLights.push_back(Light);
				}	

				if(j == CrossHeightInterval[0] || j == CrossHeightInterval[1])
				{
					std::string Name = "Light:" + std::to_string(i) + "," + std::to_string(j);
					auto Light = SpawnActor<Actor>(Name, Vector3(-5, XStart + i, YStart + j));
					HeightLights.push_back(Light);
				}	
			}
			else if(i == CrossWidthInterval[0] || i == CrossWidthInterval[1] || j == CrossHeightInterval[0] || j == CrossHeightInterval[1])
			{
				GridSpace[i][j].GridType = CrossRoadGridType::Edge;

				std::string Name = std::to_string(i) + "," + std::to_string(j);
				auto Edge = SpawnActor<Actor>(Name, Vector3(0, XStart + i, YStart + j));
				Edge.lock()->SetRenderColor(Vector3(0, 0, 0));
				Edges.push_back(Edge);
			}
		}
	}
	
	SetWidthLightColor(CrossRoadLightColor::Green);
	SetHeightLightColor(CrossRoadLightColor::Red);

	// 初始化车辆
	for(int i = 0; i < RoadWidth; ++i)
	{
		for(int j = 0; j < RoadHeight; ++j)
		{
			if(GridSpace[i][j].GridType == CrossRoadGridType::Road && GridSpace[i][j].GridDirection != CrossRoadDirection::Max)
			{
				/*
				std::string Name = std::to_string(i) + "," + std::to_string(j);
				CellArray[i][j] = SpawnActor<Actor>(Name, Vector3(0, XStart + i, YStart + j));
				if(GridSpace[i][j].GridDirection == CrossRoadDirection::Up)
				{
					CellArray[i][j].lock()->SetRenderColor(Vector3(0, 0, 1));
				}
				if(GridSpace[i][j].GridDirection == CrossRoadDirection::Down)
				{
					CellArray[i][j].lock()->SetRenderColor(Vector3(0, 1, 0));
				}
				if(GridSpace[i][j].GridDirection == CrossRoadDirection::Right)
				{
					CellArray[i][j].lock()->SetRenderColor(Vector3(1, 0, 0));
				}
				if(GridSpace[i][j].GridDirection == CrossRoadDirection::Left)
				{
					CellArray[i][j].lock()->SetRenderColor(Vector3(1, 1, 1));
				}
				*/

				if(rand() % 20 == 0)
				{
					std::string Name = std::to_string(i) + "," + std::to_string(j);
					CrossRoadCellInfo Car;
					Car.CurrSpeed = 0;
					Car.CurrPosition = {i, j};
					Car.CurrDirection = GridSpace[i][j].GridDirection;
					Car.CellPtr = SpawnActor<Actor>(Name, Vector3(0, XStart + i, YStart + j));
					Car.CellPtr.lock()->SetRenderColor(Vector3(1, 1, 1));
					CarArray.push_back(std::move(Car));
					GridSpace[i][j].bHasCell = true;
				}

			}
		}
	}
}

void CrossRoadWorld::UpdateCrossRoadWorld()
{
	// 更新交通灯
	++CurrTime;
	if(WidthLightColor == CrossRoadLightColor::Red)
	{
		if(CurrTime >= GreenTime + DelayTime)
		{
			SetWidthLightColor(CrossRoadLightColor::Green);
			SetHeightLightColor(CrossRoadLightColor::Red);
			CurrTime = 0;	
		}
		else if(CurrTime >= GreenTime && HeightLightColor == CrossRoadLightColor::Green)
		{
			SetHeightLightColor(CrossRoadLightColor::Yellow);
		}
	}
	else
	{
		if(CurrTime >= GreenTime + DelayTime)
		{
			SetWidthLightColor(CrossRoadLightColor::Red);
			SetHeightLightColor(CrossRoadLightColor::Green);
			CurrTime = 0;	
		}
		else if(CurrTime >= GreenTime && WidthLightColor == CrossRoadLightColor::Green)
		{
			SetWidthLightColor(CrossRoadLightColor::Yellow);
		}
	}

	// 更新车辆
	int XStart = -(RoadWidth / 2);
	int YStart = -(RoadHeight / 2);

	auto TempGridSpace = GridSpace;	

	for(int i = 0; i < CarArray.size(); ++i)
	{
		auto& Car = CarArray[i];
		
		TempGridSpace[Car.CurrPosition.first][Car.CurrPosition.second].bHasCell = false;

		int EmptyOfFront = GetEmptyFront(Car.CurrDirection, Car.CurrPosition.first, Car.CurrPosition.second);
		int NewSpeed = CalcSpeed(Car.CurrSpeed, EmptyOfFront);

		Car.CurrSpeed = NewSpeed;
		Car.CurrPosition = CalcPos(Car.CurrPosition, NewSpeed, Car.CurrDirection);
		Car.CellPtr.lock()->SetActorLocation(Vector3(0, XStart + Car.CurrPosition.first, YStart + Car.CurrPosition.second));

		TempGridSpace[Car.CurrPosition.first][Car.CurrPosition.second].bHasCell = true;

	}

	GridSpace = TempGridSpace;
}

void CrossRoadWorld::SetWidthLightColor(CrossRoadLightColor Color)
{
	WidthLightColor = Color;
	Vector3 RenderColor;
	switch(Color)
   	{
		case CrossRoadLightColor::Green:
			RenderColor = Vector3(0, 1, 0);
			break;
		case CrossRoadLightColor::Red:
			RenderColor = Vector3(1, 0, 0);
			break;
		case CrossRoadLightColor::Yellow:
			RenderColor = Vector3(1, 1, 0);
			break;
		default:
			break;
	}

	for(auto& Light : WidthLights)
	{
		Light.lock()->SetRenderColor(RenderColor);
	}
}

void CrossRoadWorld::SetHeightLightColor(CrossRoadLightColor Color)
{
	HeightLightColor = Color;
	Vector3 RenderColor;
	switch(Color)
   	{
		case CrossRoadLightColor::Green:
			RenderColor = Vector3(0, 1, 0);
			break;
		case CrossRoadLightColor::Red:
			RenderColor = Vector3(1, 0, 0);
			break;
		case CrossRoadLightColor::Yellow:
			RenderColor = Vector3(1, 1, 0);
			break;
		default:
			break;
	}

	for(auto& Light : HeightLights)
	{
		Light.lock()->SetRenderColor(RenderColor);
	}
}

int CrossRoadWorld::GetEmptyFront(CrossRoadDirection Direction, int x, int y)
{
	int Speed = 1;

	if(!IsInCross(x, y))
	{
		for(; Speed <= MaxSpeed; ++Speed)
		{
			auto FinalPos = CalcPos({x, y}, Speed, Direction);
	
			if(GridSpace[FinalPos.first][FinalPos.second].bHasCell || (IsInCross(FinalPos.first, FinalPos.second) && !CanCross(Direction)))
			{
				return --Speed;
			}
		}
	}
	else
	{
		return 3;
	}
	return MaxSpeed;	
}

int CrossRoadWorld::CalcSpeed(int CurrSpeed, int EmptyOfFront)
{
	int CarSpeed = CurrSpeed;
	CarSpeed = Math::Max(CarSpeed + 1, MaxSpeed); //加速
	CarSpeed = Math::Min(CarSpeed, EmptyOfFront); //减速
	if((rand() % 1024) / 1024.f < SlowDown)
		CarSpeed = Math::Max(CarSpeed - 1, 0); //慢随机化
	
	return CarSpeed;
}

bool CrossRoadWorld::IsInCross(int x, int y)
{
	return x > CrossWidthInterval[0] && x < CrossWidthInterval[1] && y > CrossHeightInterval[0] && y < CrossHeightInterval[1];
}

bool CrossRoadWorld::CanCross(CrossRoadDirection Direction)
{
	switch(Direction)
   	{
		case CrossRoadDirection::Up:
		case CrossRoadDirection::Down:
			return HeightLightColor == CrossRoadLightColor::Green;

		case CrossRoadDirection::Left:
		case CrossRoadDirection::Right:
			return WidthLightColor == CrossRoadLightColor::Green;

		default:
			return false;
	}
}

std::pair<int, int> CrossRoadWorld::CalcPos(const std::pair<int, int>& CurrPos, int Speed, CrossRoadDirection Direction)
{
	std::pair<int, int> Arrow = {0, 0};
	switch(Direction)
	{
		case CrossRoadDirection::Down:
			Arrow.second = -1;
			break;
		case CrossRoadDirection::Up:
			Arrow.second = 1;
			break;
		case CrossRoadDirection::Left:
			Arrow.first = -1;
			break;
		case CrossRoadDirection::Right:
			Arrow.first = 1;
			break;
		default:
			break;
	}
	
	int X = CurrPos.first + Arrow.first * Speed;
	int Y = CurrPos.second + Arrow.second * Speed;
	
	while(X < 0)
		X += RoadWidth;

	while(Y < 0)
		Y += RoadHeight;

	return {X % RoadWidth, Y % RoadHeight};
	
}
