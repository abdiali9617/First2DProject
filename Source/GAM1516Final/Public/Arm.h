// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arm.generated.h"

class AGun;
UCLASS()
class GAM1516FINAL_API AArm : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArm();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Actor Visual")
        class UPaperSpriteComponent* ArmSprite;

    UPROPERTY(EditAnywhere, Category = "Collision Box")
        class UBoxComponent* HitBox;

    UPROPERTY(Category = Character, VisibleAnywhere, meta = (AllowPrivateAccess))
        class USphereComponent* PickupSphere;

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        class UGunInventoryComponent* GunInventoryComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class UAudioComponent* AudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class USoundBase* AmmoPick;

    UPROPERTY(EditAnywhere, Category = Attachable)
        TSubclassOf<AGun> DefaultGunRef;

    //UPROPERTY(EditAnywhere, Category = Attachable)
        class AGun* DefaultGun;

    class AGun* CurrentGun;

    UFUNCTION()
        void NextGun();

    UFUNCTION()
        void PrevGun();

    UFUNCTION()
        void PickupGun();

    UFUNCTION()
        void FireGun();

    UFUNCTION()
        void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
};
