// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyAnimState : uint8
{
    Idle,
    Attacking,
};

UCLASS()
class GAM1516FINAL_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        class UPaperFlipbookComponent* EnemySpriteComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
        class UPaperFlipbook* AttackingFlipbook;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
        class UPaperFlipbook* IdleFlipbook;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
        EEnemyAnimState AnimState = EEnemyAnimState::Idle;

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        class UCapsuleComponent* HitBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class UArrowComponent* SpawnPoint;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class UTriggerComponent* DetectPlayer;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class UAudioComponent* AudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class USoundBase* FireGun;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class USoundBase* Walking;

    UPROPERTY(EditDefaultsOnly, Category = Projectile)
        TSubclassOf<class AEnemyBullet> EnemyBullet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
        float FireTimerDelay = 3;

    FTimerHandle SetCanShootTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(EditInstanceOnly, Category = "AI")
        bool bPatrol;

    UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
        AActor* FirstPatrolPoint;

    UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
        AActor* SecondPatrolPoint;

    AActor* CurrentPatrolPoint;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void UpdateAnimation();

    UFUNCTION()
        void MoveToNextPatrolPoint();

    UFUNCTION(BlueprintCallable, Category = "AAA")
        void SetState(EEnemyAnimState NewState) { AnimState = NewState; }

    UFUNCTION()
        void SpawnBullet();

    UFUNCTION()
        void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult &SweepResult);
    UFUNCTION()
        void EndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
