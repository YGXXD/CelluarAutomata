#include "BirthActor.h"
#include "../Math/MathLibrary.h"

BirthActor::BirthActor() : Actor()
{
    LocationTimeInterval = 0.f;
    ColorTimeInterval = 1.f;
}

void BirthActor::Update(float DeltaSeconds)
{
    if (LocationTimeInterval <= 9.f)
    {
        LocationTimeInterval += DeltaSeconds;
        SetActorLocation(Math::Linear(StartLocation, MoveToLocation, LocationTimeInterval / 9.f));
        
        if (ColorTimeInterval == 0.f)
        {
            ColorTimeInterval = 0.f; 
            StartColor = GetRenderColor();
            LerpToColor = Vector3(Math::Linear(0.5f, 1.f, Math::RandFloat()), 
                                  Math::Linear(0.5f, 1.f, Math::RandFloat()), 
                                  Math::Linear(0.5f, 1.f, Math::RandFloat()));
        }

        ColorTimeInterval += DeltaSeconds;
        if (ColorTimeInterval <= 1.f && LocationTimeInterval <= 9.f)
        {
            SetRenderColor(Math::Linear(StartColor, LerpToColor, ColorTimeInterval));
        }
        else
        {
            ColorTimeInterval = 0.f;
        }
    }
    else
    {
        if (ColorTimeInterval == 0.f)
        {
            ColorTimeInterval = 0.f; 
            StartColor = GetRenderColor();
            LerpToColor = FinalColor;
        }

        ColorTimeInterval += DeltaSeconds;
        if (ColorTimeInterval <= 1.f)
        {
            SetRenderColor(Math::Linear(StartColor, LerpToColor, ColorTimeInterval));
        }
        else
        {
            bIsUpdate = false;
        }
    }
}