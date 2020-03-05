// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPlayer.generated.h"

UENUM(BlueprintType)
enum class EAnimState : uint8
{
    Idle,
    Jumping,
    Walking,
    Shooting,
    Dying,
    Dead,
    Hit
};

UENUM(BlueprintType)		
enum class EDirectionEnum : uint8
{
    DE_Right 	UMETA(DisplayName = "Right"),
    DE_Left 	UMETA(DisplayName = "Left")
};

class AArm;
UCLASS()
class GAM1516FINAL_API AMyPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        class UPaperFlipbookComponent* PlayerSpriteComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
        class UPaperFlipbook* JumpFlipbook;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
        class UPaperFlipbook* WalkFlipbook;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
        class UPaperFlipbook* IdleFlipbook;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
        EAnimState AnimState = EAnimState::Idle;

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        class UCapsuleComponent* HitBox;

    UPROPERTY(EditAnywhere, Category = "Collision Box")
        class UBoxComponent* ArmBox;

    UPROPERTY(EditAnywhere, Category = "Player Camera")
        class UCameraComponent* PlayerCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Camera Spring", meta = (AllowPrivateAccess = "true"))
        class USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class UAudioComponent* AudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class USoundBase* FootSteps;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class USoundBase* JumpSound;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class USoundBase* LandSound;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        float JumpPower;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        float speed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
        EDirectionEnum DirectionPlayerIsFacing;

    UPROPERTY(EditAnywhere, Category = Attachable)
        TSubclassOf<AArm> ArmRef;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
        float ControllerTimerDelay = 3;

    FTimerHandle SetEnableControllerTimer;

    UFUNCTION()
        virtual	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult &Hit);

    UFUNCTION()
        void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult &SweepResult);

    UFUNCTION()
        virtual void MoveRight(float value);
 
    UFUNCTION()
        virtual void ArmMove();

    UFUNCTION(BlueprintCallable, Category = "AAA")
        void SetState(EAnimState NewState) { AnimState = NewState; }

    UFUNCTION()
        void AttachArm();

    UFUNCTION()
        void NextGun();

    UFUNCTION()
        void PrevGun();

    UFUNCTION()
        void PickupGun();

    UFUNCTION()
        void FireGun();

     void UpdateAnimation();

     void Jump();
     void Land();

     void SetEnableController();

private:
    float MovementRight;

    int Health = 50;

    bool OnCheckPoint;
    bool bJump;
    bool OnFloor = false;
    bool EnableController = true;
    bool IsAlive = true;

    FVector StartingPos;
    FVector CheckpointPos = FVector(0, 0, 0);

    AArm* m_Arm;
};
