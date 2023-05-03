#include "TrafficLightWorld.h"
#include "../Math/MathLibrary.h"
#include "CACamera.h"

void TrafficLightWorld::BeginCreate()
{
	World::BeginCreate();
	SpawnActor<CACamera>("CACamera");	
	InitTrafficLightWorld();
}

void TrafficLightWorld::WorldUpdate(float DeltaSeconds)
{
	World::WorldUpdate(DeltaSeconds);
	CurrSeconds += DeltaSeconds;
	if(CurrSeconds > TickInterval)
	{
		UpdateTrafficLightWorld();
		CurrSeconds = 0;
	}
}

void TrafficLightWorld::InitTrafficLightWorld()
{
	if(RoadWidth < 20)
		RoadWidth = 20;
	if(RoadHeight < 20)
		RoadHeight = 20;

	GridSpace = std::vector<std::vector<TrafficLightGrid>>(RoadWidth, std::vector<TrafficLightGrid>(RoadHeight, {false, TrafficLightGridType::Max, TrafficLightDirection::Max}));
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
				GridSpace[i][j].GridType = TrafficLightGridType::Road;
				if(i <= CrossWidthInterval[0] || i >= CrossWidthInterval[1] || j <= CrossHeightInterval[0] || j >= CrossHeightInterval[1])
				{
					if(i < WidthMid && i > CrossWidthInterval[0])
					{
						GridSpace[i][j].GridDirection = TrafficLightDirection::Down;
					}
					else if(i >= WidthMid && i < CrossWidthInterval[1])
					{
						GridSpace[i][j].GridDirection = TrafficLightDirection::Up;
					}
					else if(j < HeightMid && j > CrossHeightInterval[0])
					{
						GridSpace[i][j].GridDirection = TrafficLightDirection::Right;
					}
					else if(j >= HeightMid && j < CrossHeightInterval[1])
					{
						GridSpace[i][j].GridDirection = TrafficLightDirection::Left;
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
				GridSpace[i][j].GridType = TrafficLightGridType::Edge;

				std::string Name = std::to_string(i) + "," + std::to_string(j);
				auto Edge = SpawnActor<Actor>(Name, Vector3(0, XStart + i, YStart + j));
				Edge.lock()->SetRenderColor(Vector3(0, 0, 0));
				Edges.push_back(Edge);
			}
		}
	}
	
	SetWidthLightColor(TrafficLightColor::Green);
	SetHeightLightColor(TrafficLightColor::Red);

	// 初始化车辆
	for(int i = 0; i < RoadWidth; ++i)
	{
		for(int j = 0; j < RoadHeight; ++j)
		{
			if(GridSpace[i][j].GridType == TrafficLightGridType::Road && GridSpace[i][j].GridDirection != TrafficLightDirection::Max)
			{
				/*
				std::string Name = std::to_string(i) + "," + std::to_string(j);
				CellArray[i][j] = SpawnActor<Actor>(Name, Vector3(0, XStart + i, YStart + j));
				if(GridSpace[i][j].GridDirection == TrafficLightDirection::Up)
				{
					CellArray[i][j].lock()->SetRenderColor(Vector3(0, 0, 1));
				}
				if(GridSpace[i][j].GridDirection == TrafficLightDirection::Down)
				{
					CellArray[i][j].lock()->SetRenderColor(Vector3(0, 1, 0));
				}
				if(GridSpace[i][j].GridDirection == TrafficLightDirection::Right)
				{
					CellArray[i][j].lock()->SetRenderColor(Vector3(1, 0, 0));
				}
				if(GridSpace[i][j].GridDirection == TrafficLightDirection::Left)
				{
					CellArray[i][j].lock()->SetRenderColor(Vector3(1, 1, 1));
				}
				*/

				if(rand() % 20 == 0)
				{
					std::string Name = std::to_string(i) + "," + std::to_string(j);
					TrafficLightCellInfo Car;
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

void TrafficLightWorld::UpdateTrafficLightWorld()
{
	// 更新交通灯
	++CurrTime;
	if(WidthLightColor == TrafficLightColor::Red)
	{
		if(CurrTime >= GreenTime + DelayTime)
		{
			SetWidthLightColor(TrafficLightColor::Green);
			SetHeightLightColor(TrafficLightColor::Red);
			CurrTime = 0;	
		}
		else if(CurrTime >= GreenTime && HeightLightColor == TrafficLightColor::Green)
		{
			SetHeightLightColor(TrafficLightColor::Yellow);
		}
	}
	else
	{
		if(CurrTime >= GreenTime + DelayTime)
		{
			SetWidthLightColor(TrafficLightColor::Red);
			SetHeightLightColor(TrafficLightColor::Green);
			CurrTime = 0;	
		}
		else if(CurrTime >= GreenTime && WidthLightColor == TrafficLightColor::Green)
		{
			SetWidthLightColor(TrafficLightColor::Yellow);
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

void TrafficLightWorld::SetWidthLightColor(TrafficLightColor Color)
{
	WidthLightColor = Color;
	Vector3 RenderColor;
	switch(Color)
   	{
		case TrafficLightColor::Green:
			RenderColor = Vector3(0, 1, 0);
			break;
		case TrafficLightColor::Red:
			RenderColor = Vector3(1, 0, 0);
			break;
		case TrafficLightColor::Yellow:
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

void TrafficLightWorld::SetHeightLightColor(TrafficLightColor Color)
{
	HeightLightColor = Color;
	Vector3 RenderColor;
	switch(Color)
   	{
		case TrafficLightColor::Green:
			RenderColor = Vector3(0, 1, 0);
			break;
		case TrafficLightColor::Red:
			RenderColor = Vector3(1, 0, 0);
			break;
		case TrafficLightColor::Yellow:
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

int TrafficLightWorld::GetEmptyFront(TrafficLightDirection Direction, int x, int y)
{
	int Speed = 1;

	bool bInCross = IsInCross(x, y);
	for(; Speed <= MaxSpeed; ++Speed)
	{
		auto FinalPos = CalcPos({x, y}, Speed, Direction);

		if(bInCross)
		{
			if(GridSpace[FinalPos.first][FinalPos.second].bHasCell)
			{
				return --Speed;
			}
		}
		else
		{
			if(GridSpace[FinalPos.first][FinalPos.second].bHasCell || (IsInCross(FinalPos.first, FinalPos.second) && !CanCross(Direction)))
			{
				return --Speed;
			}
		}
	}
	
	return MaxSpeed;	
}

int TrafficLightWorld::CalcSpeed(int CurrSpeed, int EmptyOfFront)
{
	int CarSpeed = CurrSpeed;
	CarSpeed = Math::Min(CarSpeed + 1, MaxSpeed); //加速
	CarSpeed = Math::Min(CarSpeed, EmptyOfFront); //减速
	if((rand() % 1024) / 1024.f < SlowDown)
		CarSpeed = Math::Max(CarSpeed - 1, 0); //慢随机化
	
	return CarSpeed;
}

bool TrafficLightWorld::IsInCross(int x, int y)
{
	return x > CrossWidthInterval[0] && x < CrossWidthInterval[1] && y > CrossHeightInterval[0] && y < CrossHeightInterval[1];
}

bool TrafficLightWorld::CanCross(TrafficLightDirection Direction)
{
	switch(Direction)
   	{
		case TrafficLightDirection::Up:
		case TrafficLightDirection::Down:
			return HeightLightColor == TrafficLightColor::Green;

		case TrafficLightDirection::Left:
		case TrafficLightDirection::Right:
			return WidthLightColor == TrafficLightColor::Green;

		default:
			return false;
	}
}

std::pair<int, int> TrafficLightWorld::CalcPos(const std::pair<int, int>& CurrPos, int Speed, TrafficLightDirection Direction)
{
	std::pair<int, int> Arrow = {0, 0};
	switch(Direction)
	{
		case TrafficLightDirection::Down:
			Arrow.second = -1;
			break;
		case TrafficLightDirection::Up:
			Arrow.second = 1;
			break;
		case TrafficLightDirection::Left:
			Arrow.first = -1;
			break;
		case TrafficLightDirection::Right:
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
