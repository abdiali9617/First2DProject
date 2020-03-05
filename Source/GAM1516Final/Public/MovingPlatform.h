// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPlatform.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class GAM1516FINAL_API AMovingPlatform : public AMyPlatform
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
    float RunningTime;

public:
    virtual void Tick(float DeltaTime) override;
};
