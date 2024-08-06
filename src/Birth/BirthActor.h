#ifndef BIRTH_ACTOR_H
#define BIRTH_ACTOR_H

#include "../Core/Actor.h"

class BirthActor : public Actor
{
public:
    BirthActor();
    virtual void Update(float DeltaSeconds) override;

    float TimeInterval;
    Vector3 StartLocation;
    Vector3 MoveToLocation;
};

#endif