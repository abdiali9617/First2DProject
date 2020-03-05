// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayer.h"
#include "MyPlayerController.h"
#include "Classes/PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbook.h"
#include "ConstructorHelpers.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "TimerManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Arm.h"
#include "Gun.h"
#include "GunInventoryComponent.h"
#include "MyGameStateBase.h"
#include "MyBlueprintFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Engine/Engine.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    HitBox = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
    HitBox->SetCollisionProfileName("BlockAll");
    HitBox->SetNotifyRigidBodyCollision(true);
    HitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    HitBox->SetSimulatePhysics(true);

    HitBox->OnComponentHit.AddDynamic(this, &AMyPlayer::OnHit);
    HitBox->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::BeginOverlap);
    UMyBlueprintFunctionLibrary::LockPhysicsTo2DAxis(HitBox);
    RootComponent = HitBox;

    UPaperSpriteComponent* comp = Cast<UPaperSpriteComponent>(GetComponentByClass(UPaperSpriteComponent::StaticClass()));
    if (comp)
    {
        comp->DestroyComponent();
        comp->SetActive(false);
    }

    PlayerSpriteComponent = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(APaperCharacter::SpriteComponentName);
    if (PlayerSpriteComponent)
    {
        PlayerSpriteComponent->SetupAttachment(RootComponent);
        PlayerSpriteComponent->SetCollisionProfileName("NoCollision");
        PlayerSpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        PlayerSpriteComponent->SetGenerateOverlapEvents(false);
    }

    ArmBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    ArmBox->SetupAttachment(RootComponent);
    ArmBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ArmBox->SetCollisionProfileName("OverlapAll");

    SetState(EAnimState::Idle);

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
    SpringArmComponent->bDoCollisionTest = false;
    SpringArmComponent->bInheritPitch = false;
    SpringArmComponent->bInheritYaw = false;
    SpringArmComponent->bInheritRoll = false;
    SpringArmComponent->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
    SpringArmComponent->SetupAttachment(RootComponent);

    PlayerCamera = CreateDefaultSubobject<UCameraComponent>("Player Follow Camera");
    PlayerCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
    PlayerCamera->SetOrthoWidth(2000.0f);
    PlayerCamera->SetupAttachment(SpringArmComponent);

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEmitter"));
    AudioComponent->bAutoActivate = false;
    AudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    AudioComponent->SetupAttachment(RootComponent);

    DirectionPlayerIsFacing = EDirectionEnum::DE_Right;
    Tags.Add("Player");
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
    Super::BeginPlay();
    if (PlayerSpriteComponent != nullptr)
        PlayerSpriteComponent->Stop();
    AttachArm();
    StartingPos = GetActorLocation();
    CheckpointPos = StartingPos;
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateAnimation();
    ArmMove();

    if (EnableController == false)
    {
        GetWorldTimerManager().SetTimer(SetEnableControllerTimer, this, &AMyPlayer::SetEnableController, ControllerTimerDelay, true);
        EnableController = true;
    }
}

void AMyPlayer::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
    APlayerController* PlayerController = Cast<APlayerController>(GetController());

    if (OtherActor != nullptr)
    {
        if (OtherActor->ActorHasTag(FName("GameObject")))
        {
            Land();
        }
        if (OtherActor->ActorHasTag(FName("EnemyBullet")))
        {
            AMyGameStateBase* GameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
            int Health = GameState->GetPlayerHealth();
            if (Health == 0)
            {
                EnableController = false;
                PlayerController->DisableInput(PlayerController);
                SetActorLocation(CheckpointPos);
                Health = 50;
                GameState->SetPlayerHealth(Health);
            }
        }
       
    }
}

void AMyPlayer::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor->ActorHasTag(FName("Checkpoint")))
    {
        CheckpointPos = GetActorLocation();
    }
}

void AMyPlayer::MoveRight(float value)
{
    MovementRight = value;
    if (EnableController == true)
    {
        if (MovementRight != 0)
        {
            if (MovementRight > 0)
            {
                PlayerSpriteComponent->SetRelativeRotation(FRotator(FRotator::ZeroRotator));
                DirectionPlayerIsFacing = EDirectionEnum::DE_Right;
            }

            if (MovementRight < 0)
            {
                PlayerSpriteComponent->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
                DirectionPlayerIsFacing = EDirectionEnum::DE_Left;
            }

            HitBox->BodyInstance.AddForce(FVector(1.0f, 0.0f, 0.0f) * MovementRight * speed);
            AudioComponent->SetSound(FootSteps);
            AudioComponent->Play();
        }

        if (AnimState == EAnimState::Idle || AnimState == EAnimState::Walking)
        {
            if (MovementRight != 0.f)
            {
                SetState(EAnimState::Walking);
            }
            else
            {
                SetState(EAnimState::Idle);
            }
        }
    }
}

void AMyPlayer::ArmMove()
{
    if (ArmRef != nullptr)
    {
        bool MouseInWindow = true;

        APlayerController* PlayerController = Cast<APlayerController>(GetController());
        PlayerController->bShowMouseCursor = true;
        PlayerController->bEnableClickEvents = true;
        PlayerController->bEnableMouseOverEvents = true;

        FVector WorldPosition, WorldDirection;
        if (PlayerController != nullptr)
        {
            float LocationX, LocationY;
            PlayerController->GetMousePosition(LocationX, LocationY);
            FVector MousePosition(LocationX, LocationY, 0);

            MouseInWindow = MousePosition.X > 0.0f && MousePosition.Y > 0.0f;

            PlayerController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);

            float y = MousePosition.Y - m_Arm->GetActorRotation().Pitch;
            float x = MousePosition.X - m_Arm->GetActorRotation().Pitch;
            float MouseAngle = 180 / PI * (FMath::Atan2(y, x));
            MouseAngle = FMath::ClampAngle(MouseAngle, 0.0f, 180.0f);
            if (MouseInWindow)
            {
                m_Arm->SetActorRotation(FRotator(-(MouseAngle - 25), m_Arm->GetActorRotation().Yaw, m_Arm->GetActorRotation().Roll));
            }
        }
    }
}

void AMyPlayer::NextGun()
{
    if (ArmRef != nullptr)
    {
        m_Arm->NextGun();
    }
}

void AMyPlayer::PrevGun()
{
    if (ArmRef != nullptr)
    {
        m_Arm->PrevGun();
    }
}

void AMyPlayer::PickupGun()
{
    if (ArmRef != nullptr)
    {
        m_Arm->PickupGun();
    }
}


void AMyPlayer::AttachArm()
{
    if (ArmRef != nullptr)
    {
        m_Arm = Cast<AArm>(GetWorld()->SpawnActor(ArmRef));
        m_Arm->AttachToComponent(PlayerSpriteComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "arm");
    }
}

void AMyPlayer::FireGun()
{
    if (EnableController == true)
    {
        if (ArmRef != nullptr)
        {
            m_Arm->FireGun();
        }
    }
}

void AMyPlayer::UpdateAnimation()
{
    float Velocity = GetVelocity().Size();
    if (Velocity > 0)
    {
        if (AnimState == EAnimState::Jumping)
        {

            PlayerSpriteComponent->SetFlipbook(JumpFlipbook);
            PlayerSpriteComponent->SetLooping(false);
            PlayerSpriteComponent->Play();
        }
        else
        {

            PlayerSpriteComponent->SetFlipbook(WalkFlipbook);
            PlayerSpriteComponent->SetLooping(true);
            PlayerSpriteComponent->Play();
        }
    }
    else
    {
        PlayerSpriteComponent->Stop();
        PlayerSpriteComponent->SetFlipbook(IdleFlipbook);
        PlayerSpriteComponent->SetLooping(true);
        PlayerSpriteComponent->Play();
    }
}

void AMyPlayer::Jump()
{
    if (EnableController == true)
    {
        if (OnFloor)
        {
            OnFloor = false;
            HitBox->BodyInstance.AddForce(FVector(0.0f, 0.0f, 1.0f) * speed * JumpPower);
            AudioComponent->SetSound(JumpSound);
            AudioComponent->Play();
        }

        if (AnimState == EAnimState::Walking || AnimState == EAnimState::Idle)
            SetState(EAnimState::Jumping);
    }
}

void AMyPlayer::Land()
{
    if (EnableController == true)
    {
        if (!OnFloor)
        {
            OnFloor = true;
            AudioComponent->SetSound(LandSound);
            AudioComponent->Play();
            SetState(EAnimState::Idle);
        }
    }
}

void AMyPlayer::SetEnableController()
{
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    PlayerController->EnableInput(PlayerController);
}

