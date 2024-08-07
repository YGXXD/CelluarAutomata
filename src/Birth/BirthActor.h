#ifndef BIRTH_ACTOR_H
#define BIRTH_ACTOR_H

#include "../Core/Actor.h"

class BirthActor : public Actor
{
public:
    BirthActor();
    virtual void Update(float DeltaSeconds) override;

    float LocationTimeInterval;
    float ColorTimeInterval;
    Vector3 StartColor;
    Vector3 LerpToColor;
    Vector3 StartLocation;
    Vector3 MoveToLocation;
    Vector3 FinalColor;
};

#endif