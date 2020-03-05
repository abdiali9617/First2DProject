// Fill out your copyright notice in the Description page of Project Settings.

#include "Tilan.h"
#include "Classes/PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperCharacter.h"
#include "MyPlayer.h"
#include "MyPlayerController.h"
#include "Components/CapsuleComponent.h"
#include <Components/ArrowComponent.h>
#include "EnemyBullet.h"
#include "TriggerComponent.h"
#include <Components/SkeletalMeshComponent.h>
#include "MyGameStateBase.h"
#include "Components/AudioComponent.h"
#include "MyBlueprintFunctionLibrary.h"
#include "Engine/Engine.h"

// Sets default values
ATilan::ATilan()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    HitBox = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
    HitBox->SetCollisionProfileName("BlockAll");
    HitBox->SetNotifyRigidBodyCollision(true);
    HitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    HitBox->SetSimulatePhysics(false);
    UMyBlueprintFunctionLibrary::LockPhysicsTo2DAxis(HitBox);

    RootComponent = HitBox;

    UPaperSpriteComponent* comp = Cast<UPaperSpriteComponent>(GetComponentByClass(UPaperSpriteComponent::StaticClass()));
    if (comp)
    {
        comp->DestroyComponent();
        comp->SetActive(false);
    }

    TilanSpriteComponent = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(APaperCharacter::SpriteComponentName);
    if (TilanSpriteComponent)
    {
        TilanSpriteComponent->SetupAttachment(RootComponent);
        TilanSpriteComponent->SetCollisionProfileName("NoCollision");
        TilanSpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        TilanSpriteComponent->SetGenerateOverlapEvents(false);
    }

    DetectPlayer = CreateDefaultSubobject<UTriggerComponent>("TriggerComponent");
    DetectPlayer->SetCollisionProfileName("Overlap All");
    DetectPlayer->SetNotifyRigidBodyCollision(true);
    DetectPlayer->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    DetectPlayer->OnComponentBeginOverlap.AddDynamic(this, &ATilan::BeginOverlap);
    DetectPlayer->OnComponentEndOverlap.AddDynamic(this, &ATilan::EndOverlap);
    DetectPlayer->SetSimulatePhysics(false);
    DetectPlayer->SetGenerateOverlapEvents(true);
    DetectPlayer->SetupAttachment(RootComponent);

    SpawnPoint = CreateDefaultSubobject<UArrowComponent>("SpawnPoint");
    SpawnPoint->SetupAttachment(RootComponent);

    MusicComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEmitter"));
    MusicComponent->bAutoActivate = false;
    MusicComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    MusicComponent->SetupAttachment(RootComponent);

    Tags.Add("Tilan");
}

// Called when the game starts or when spawned
void ATilan::BeginPlay()
{
	Super::BeginPlay();

    //Airsoft = Cast<AMyPlayer>(PlayerRef);
    GetWorldTimerManager().SetTimer(SetCanShootTimer, this, &ATilan::SpawnPaintBall, FireTimerDelay, true);

}

// Called every frame
void ATilan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    UpdateAnimation();
    //InRange();
}

void ATilan::UpdateAnimation()
{
    if (AnimState == ETilanAnimState::Vaping)
    {
        TilanSpriteComponent->SetFlipbook(VapingFlipbook);
        TilanSpriteComponent->SetLooping(true);
        TilanSpriteComponent->Play();
    }
    else
    {
        TilanSpriteComponent->SetFlipbook(AttackingFlipbook);
        TilanSpriteComponent->SetLooping(true);
        TilanSpriteComponent->Play();
    }
}

void ATilan::SpawnPaintBall()
{
    if (PaintBall != nullptr)
    {
        if(AirsoftIsInRange == true)
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
                    World->SpawnActor<AEnemyBullet>(PaintBall,
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

void ATilan::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor->ActorHasTag(FName("Player")))
    {
        if (AnimState == ETilanAnimState::Vaping)
        {
            AirsoftIsInRange = true;
            SetState(ETilanAnimState::Attacking);
            MusicComponent->SetSound(UltimateBattle);
            MusicComponent->Play();
        }
    }
}

void ATilan::EndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor->ActorHasTag(FName("Player")))
    {
        if (AnimState == ETilanAnimState::Attacking)
        {
            AirsoftIsInRange = false;
            SetState(ETilanAnimState::Vaping);
            MusicComponent->Stop();
        }
    }
}
