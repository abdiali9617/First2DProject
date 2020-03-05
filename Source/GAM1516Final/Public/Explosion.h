// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

enum class EEnemyAnimState : uint8
{
    Exploding,
};
UCLASS()
class GAM1516FINAL_API AExplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosion();

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        class UPaperFlipbookComponent* ExplosionSpriteComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
        class UPaperFlipbook* ExplosionFlipbook;

    UPROPERTY(Category = Collision, VisibleAnywhere,
        meta = (AllowPrivateAccess))
        class USphereComponent* HitBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
