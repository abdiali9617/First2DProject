// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "EnemyBullet.h"
#include "Classes/PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperCharacter.h"
#include "MyPlayer.h"
#include "MyPlayerController.h"
#include "TriggerComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include <Components/ArrowComponent.h>
#include "Components/AudioComponent.h"
#include <Components/SkeletalMeshComponent.h>
#include "MyBlueprintFunctionLibrary.h"


// Sets default values
AEnemy::AEnemy()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    HitBox = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
    HitBox->SetCollisionProfileName("BlockAll");
    HitBox->SetNotifyRigidBodyCollision(true);
    HitBox->SetSimulatePhysics(false);
    UMyBlueprintFunctionLibrary::LockPhysicsTo2DAxis(HitBox);
    HitBox->GetBodyInstance()->bLockZTranslation = true;

    RootComponent = HitBox;

    UPaperSpriteComponent* comp = Cast<UPaperSpriteComponent>(GetComponentByClass(UPaperSpriteComponent::StaticClass()));
    if (comp)
    {
        comp->DestroyComponent();
        comp->SetActive(false);
    }

    EnemySpriteComponent = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(APaperCharacter::SpriteComponentName);
    if (EnemySpriteComponent)
    {
        EnemySpriteComponent->SetupAttachment(RootComponent);
        EnemySpriteComponent->SetCollisionProfileName("NoCollision");
        EnemySpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        EnemySpriteComponent->SetGenerateOverlapEvents(false);
    }

    DetectPlayer = CreateDefaultSubobject<UTriggerComponent>("TriggerComponent");
    DetectPlayer->SetCollisionProfileName("Overlap All");
    DetectPlayer->SetNotifyRigidBodyCollision(true);
    DetectPlayer->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    DetectPlayer->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::BeginOverlap);
    DetectPlayer->OnComponentEndOverlap.AddDynamic(this, &AEnemy::EndOverlap);
    DetectPlayer->SetSimulatePhysics(false);
    DetectPlayer->SetGenerateOverlapEvents(true);
    DetectPlayer->SetupAttachment(RootComponent);

    SpawnPoint = CreateDefaultSubobject<UArrowComponent>("SpawnPoint");
    SpawnPoint->SetupAttachment(RootComponent);

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEmitter"));
    AudioComponent->bAutoActivate = false;
    AudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    AudioComponent->SetupAttachment(RootComponent);

    Tags.Add("Enemy");

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(SetCanShootTimer, this, &AEnemy::SpawnBullet, FireTimerDelay, true);
    CurrentPatrolPoint = SecondPatrolPoint;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (AnimState == EEnemyAnimState::Idle)
    {
        MoveToNextPatrolPoint();
        if (CurrentPatrolPoint != nullptr)
        {
            FVector TargetLocation = FMath::VInterpConstantTo(GetActorLocation(), CurrentPatrolPoint->GetActorLocation(), DeltaTime, 100.0f);
            SetActorLocation(TargetLocation);
            AudioComponent->SetSound(Walking);
            AudioComponent->Play();
        }
    }
    UpdateAnimation();
}

void AEnemy::UpdateAnimation()
{
    if (AnimState == EEnemyAnimState::Idle)
    {
        EnemySpriteComponent->SetFlipbook(IdleFlipbook);
        EnemySpriteComponent->SetLooping(true);
        EnemySpriteComponent->Play();
    }
    else
    {
        EnemySpriteComponent->SetFlipbook(AttackingFlipbook);
        EnemySpriteComponent->SetLooping(true);
        EnemySpriteComponent->Play();
    }
}

void AEnemy::MoveToNextPatrolPoint()
{
    if (AnimState == EEnemyAnimState::Idle)
    {
        FVector curPos = GetActorLocation();

        if (CurrentPatrolPoint == FirstPatrolPoint && curPos.X == FirstPatrolPoint->GetActorLocation().X)
        {
            CurrentPatrolPoint = SecondPatrolPoint;
            HitBox->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
        }

        else if (CurrentPatrolPoint == SecondPatrolPoint && curPos.X == SecondPatrolPoint->GetActorLocation().X)
        {
            CurrentPatrolPoint = FirstPatrolPoint;
            HitBox->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
        }
    }
}

void AEnemy::SpawnBullet()
{
    if (EnemyBullet != nullptr)
    {
        {
            if (AnimState == EEnemyAnimState::Attacking)
            {
                UWorld* const World = GetWorld();
                if (World)
                {
                    FActorSpawnParameters SpawnParams;
                    SpawnParams.Owner = this;
                    SpawnParams.SpawnCollisionHandlingOverride =
                        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

                    FTransform SpawnTransform =
                        SpawnPoint->GetComponentTransform();
                    AEnemyBullet* SpawnedActor =
                        World->SpawnActor<AEnemyBullet>(EnemyBullet,
                            SpawnTransform,
                            SpawnParams);
                    if (SpawnedActor)
                    {
                        FVector direction = FRotationMatrix(SpawnTransform.Rotator()).GetScaledAxis(EAxis::X);
                        SpawnedActor->SetFireDirection(direction);
                    }
                }
            }
        }
    }
    AudioComponent->SetSound(FireGun);
    AudioComponent->Play();
}

void AEnemy::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor->ActorHasTag(FName("Player")))
    {
        if (AnimState == EEnemyAnimState::Idle)
        {
            SetState(EEnemyAnimState::Attacking);
        }
    }
}

void AEnemy::EndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor->ActorHasTag(FName("Player")))
    {
        if (AnimState == EEnemyAnimState::Attacking)
        {
            SetState(EEnemyAnimState::Idle);
        }
    }
}

