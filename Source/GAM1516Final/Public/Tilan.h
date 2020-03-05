// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tilan.generated.h"

UENUM(BlueprintType)
enum class ETilanAnimState : uint8
{
    Vaping,
    Attacking,
};

UCLASS()
class GAM1516FINAL_API ATilan : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATilan();

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        class UPaperFlipbookComponent* TilanSpriteComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
        class UPaperFlipbook* AttackingFlipbook;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
        class UPaperFlipbook* VapingFlipbook;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
        ETilanAnimState AnimState = ETilanAnimState::Vaping;

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        class UCapsuleComponent* HitBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class UArrowComponent* SpawnPoint;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class UTriggerComponent* DetectPlayer;

    UPROPERTY(EditDefaultsOnly, Category = Projectile)
        TSubclassOf<class AEnemyBullet> PaintBall;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class UAudioComponent* MusicComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class USoundBase* UltimateBattle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
        float FireTimerDelay = 3;

    FTimerHandle SetCanShootTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    class AMyPlayer* Airsoft;

    bool AirsoftIsInRange = false;

    int Health = 100;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void UpdateAnimation();

    UFUNCTION(BlueprintCallable, Category = "AAA")
        void SetState(ETilanAnimState NewState) { AnimState = NewState; }

    UFUNCTION()
        void SpawnPaintBall();

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
