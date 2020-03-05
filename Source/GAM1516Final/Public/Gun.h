// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class GAM1516FINAL_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
        class USphereComponent* HitBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class UPaperSpriteComponent* GunSprite;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class UArrowComponent* SpawnPoint;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        int MaximumAmmo;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        int CurrentAmmo;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class UAudioComponent* AudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class USoundBase* FireSound;

    UPROPERTY(EditDefaultsOnly, Category = Projectile)
        TSubclassOf<class ABullet> Bullet;

    UFUNCTION()
        virtual void Disable();

    UFUNCTION()
        virtual void Enable();

    UFUNCTION()
        void Fire();

    FVector GetSpawnLocation() const;

    void Equip(class AArm* Arm);

    void UnEquip();

    virtual void SpawnBullet();
};
