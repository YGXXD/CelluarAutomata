#include "Birth2025World.h"
#include "Birth2025Camera.h"
#include "../Math/MathLibrary.h"
#include <algorithm>
#include <iostream>

#include "../ThirdParty/stb_image.h"

Birth2025World::Birth2025World() : World()
{

}

Birth2025World::~Birth2025World()
{

}

void Birth2025World::BeginCreate()
{
	World::BeginCreate();
	SpawnActor<Birth2025Camera>("Birth2025Camera");
	CellArray = std::vector<std::weak_ptr<BirthActor>>();
	BirthDayInit(50, 50, -40, 20, std::string(IMAGE_PATH) + "birth_x.png", true);
	BirthDayInit(50, 50, -5, 20, std::string(IMAGE_PATH) + "birth_x.png", true);
	BirthDayInit(50, 50, 30, 20, std::string(IMAGE_PATH) + "birth_d.png", true);
	BirthDayInit(50, 50, -45, -20, std::string(IMAGE_PATH) + "birth_1.png", true);
	BirthDayInit(50, 50, -15, -20, std::string(IMAGE_PATH) + "birth_2.png", true);
	BirthDayInit(50, 50, 15, -20, std::string(IMAGE_PATH) + "birth_3.png", true);
	BirthDayInit(50, 50, 45, -20, std::string(IMAGE_PATH) + "birth_4.png", true);
	BirthDayInit(60, 60, 65, 22, std::string(IMAGE_PATH) + "nfl.png", false);
}

void Birth2025World::WorldUpdate(float DeltaSeconds)
{
	World::WorldUpdate(DeltaSeconds);
	if (LangdingStartTime < CellStartInterval)
	{
		LangdingStartTime += DeltaSeconds;
	}
	else
	{
		LangdingStartTime = 0.f;
		for(int i = 0; i < 50; ++i)
		{
			if (CellIndex < CellArray.size())
			{
				if(!CellArray[CellIndex].expired())
					CellArray[CellIndex++].lock()->SetIsUpdate(true);
			}
			else
				break;
		}
	}
}

void Birth2025World::BeginDestroy()
{
	World::BeginDestroy();
}

void Birth2025World::BirthDayInit(int Width, int Height, float ScreenX, 
	float ScreenY, const std::string& ImagePath, bool RenderAsText, float Scale)
{
    int ImageWidth, ImageHeight, Channels;
    unsigned char* data = stbi_load(ImagePath.c_str(), &ImageWidth, &ImageHeight, &Channels, 0);
	if (data == nullptr)
		std::cout << "数字加载失败" << std::endl;

	float XStart = -(Width * Scale / 2);
	float YStart = -(Height * Scale / 2);
	for(int i = 0; i < Width; i++)
	{
		for(int j = 0; j < Height; j++)
		{
			float U = static_cast<float>(i) / static_cast<float>(Width);
			float V = static_cast<float>(Height - 1 - j) / static_cast<float>(Height);

			int ImageX = static_cast<int>(U * ImageWidth);
			int ImageY = static_cast<int>(V * ImageHeight);
			int ImageIndex = ImageX + ImageY * ImageWidth;
			int ColorIndex = Channels * ImageIndex;
			uint8_t r = data[ColorIndex];
			uint8_t g = data[ColorIndex + 1];
			uint8_t b = data[ColorIndex + 2];
			uint8_t a = data[ColorIndex + 3];

			bool BirthProbability = RenderAsText ? (r < 128 && g < 128 && b < 128 && a > 0):(a > 0);
			if(BirthProbability)
			{
				std::string Name = std::to_string(i) + "," + std::to_string(j);
				float theta = Math::Linear(0.f, 2 * 3.1415926f, Math::RandFloat());
				float radius = Math::Linear(300, 500, Math::RandFloat());
				Vector3 Location = Vector3(Math::Linear(-50, 5, Math::RandFloat()), radius * Math::Cos(theta), radius * Math::Sin(theta));
				Vector3 Offset = Vector3(0, i, j) * Scale;
				std::weak_ptr<BirthActor> actor = SpawnActor<BirthActor>(Name, Location, Vector3(Scale, Scale, Scale));
				actor.lock()->TimeLineScalar = 3.f;
				actor.lock()->StartLocation = Location;
				actor.lock()->MoveToLocation = Vector3(0, XStart + ScreenX, YStart + ScreenY) + Offset;
				actor.lock()->SetRenderColor(Vector3(1, 1, 1));
				if (RenderAsText)
					actor.lock()->FinalColor = CellColor;
				else 	
					actor.lock()->FinalColor = Vector3((float)(data[ColorIndex]) / (float)0xff, (float)(data[ColorIndex + 1]) / (float)0xff, (float)(data[ColorIndex + 2]) / (float)0xff);
				actor.lock()->SetIsUpdate(false);
				CellArray.push_back(actor);
			}
		}
	}
	stbi_image_free(data);
}
