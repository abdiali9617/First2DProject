// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPlatform.generated.h"

UCLASS()
class GAM1516FINAL_API AMyPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Collision Box")
        class UBoxComponent* HitBox;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Platform Visual")
        class UPaperSpriteComponent* PlatformSprite;
	
};
