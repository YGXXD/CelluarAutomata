#include "BirthActor.h"
#include "../Math/MathLibrary.h"

BirthActor::BirthActor() : Actor()
{
    TimeInterval = 0.f;
}

void BirthActor::Update(float DeltaSeconds)
{
    if (TimeInterval < 10.f)
    {
        TimeInterval += DeltaSeconds;
        SetActorLocation(Math::Linear(StartLocation, MoveToLocation, TimeInterval * 0.1f));
    }
    else
    {
        bIsUpdate = false;
    }
}