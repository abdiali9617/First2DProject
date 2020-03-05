// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

UENUM(BlueprintType)
enum class ECheckpointState : uint8
{
    Down,
    Up,
};

UCLASS()
class GAM1516FINAL_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Collision Box")
        class UBoxComponent* HitBox;

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        class UPaperFlipbookComponent* CheckpointSpriteComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
        class UPaperFlipbook* DownFlipbook;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
        class UPaperFlipbook* UpFlipbook;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
        ECheckpointState CheckpointState = ECheckpointState::Down;
	
    UFUNCTION()
        void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult &SweepResult);

    UFUNCTION(BlueprintCallable, Category = "AAA")
        void SetState(ECheckpointState NewState) { CheckpointState = NewState; }
	
    void UpdateAnimation();
};
