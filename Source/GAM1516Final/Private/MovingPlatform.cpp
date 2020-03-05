// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"
#include "MyPlatform.h"
#include "PaperSpriteComponent.h"

void AMovingPlatform::Tick(float DeltaTime)
{
    FVector NewLocation = GetActorLocation();
    float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
    NewLocation.X += DeltaHeight * 100.0f;
    RunningTime += DeltaTime;
    SetActorLocation(NewLocation);
}
