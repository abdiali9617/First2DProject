// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GAM1516FINAL_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
    virtual void OnPossess(APawn* aPawn) override;

    virtual void OnUnPossess() override;

    virtual void SetupInputComponent() override;

    void MoveRight(float value);

    void NextGun();

    void PrevGun();

    void PickupGun();

    void FireGun();
	
    void Jump();
};
