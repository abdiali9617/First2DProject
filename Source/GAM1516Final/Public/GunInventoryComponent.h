// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAM1516FINAL_API UGunInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    int Iterator = 0;

    class AMyPlayer* Player;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
        class AGun* CurrentGun;

    UPROPERTY(VisibleAnywhere)
        TArray< class AGun* > GunArray;

    class AArm* Arm;

    void AddToGunInventory(class AGun* gun);

    void ResetCurrentInventory();

    int GetGunCount();

    UFUNCTION(BlueprintCallable, Category = "Inventory")
        virtual void NextGun();

    UFUNCTION(BlueprintCallable, Category = "Inventory")
        virtual void PreviousGun();

    UFUNCTION(BlueprintCallable, Category = "Inventory")
        class AGun* GetCurrentGun() const;
};
