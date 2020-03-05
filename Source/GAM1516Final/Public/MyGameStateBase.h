// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class GAM1516FINAL_API AMyGameStateBase : public AGameStateBase
{
    GENERATED_BODY()
private:
    int m_PlayerHealth = 50;
    int m_TilanHealth = 100;

    int m_Ammo = 0;

public:

    UFUNCTION(BlueprintCallable, Category = "Health")
    int GetPlayerHealth();

    UFUNCTION(BlueprintCallable, Category = "Health")
    int GetTilanHealth();

    UFUNCTION(BlueprintCallable, Category = "Ammo")
    int GetAmmo();
	
    void SetPlayerHealth(int health);

    void SetTilanHealth(int health);

    void SetAmmo(int ammo);
};
