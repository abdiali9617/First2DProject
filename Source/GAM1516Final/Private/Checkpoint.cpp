// Fill out your copyright notice in the Description page of Project Settings.

#include "Checkpoint.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperCharacter.h"
#include "Components/BoxComponent.h"
#include "MyBlueprintFunctionLibrary.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    HitBox = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
    HitBox->SetCollisionProfileName("Overlap All");
    HitBox->SetNotifyRigidBodyCollision(true);
    HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    HitBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::BeginOverlap);
    HitBox->SetSimulatePhysics(false);

    UMyBlueprintFunctionLibrary::LockPhysicsTo2DAxis(HitBox);
    HitBox->GetBodyInstance()->bLockXTranslation = true;
    HitBox->GetBodyInstance()->bLockZTranslation = true;

    RootComponent = HitBox;

    UPaperSpriteComponent* comp = Cast<UPaperSpriteComponent>(GetComponentByClass(UPaperSpriteComponent::StaticClass()));
    if (comp)
    {
        comp->DestroyComponent();
        comp->SetActive(false);
    }

    CheckpointSpriteComponent = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(APaperCharacter::SpriteComponentName);
    if (CheckpointSpriteComponent)
    {
        CheckpointSpriteComponent->SetupAttachment(RootComponent);
        CheckpointSpriteComponent->SetCollisionProfileName("NoCollision");
        CheckpointSpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        CheckpointSpriteComponent->SetGenerateOverlapEvents(false);
    }

    Tags.Add("Checkpoint");
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    UpdateAnimation();
}

void ACheckpoint::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor->ActorHasTag(FName("Player")))
    {
        if (CheckpointState == ECheckpointState::Down)
        {
            SetState(ECheckpointState::Up);
            HitBox->SetCollisionProfileName("No Collision");
            HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }
}

void ACheckpoint::UpdateAnimation()
{
    if (CheckpointState == ECheckpointState::Down)
    {

        CheckpointSpriteComponent->SetFlipbook(DownFlipbook);
        CheckpointSpriteComponent->SetLooping(false);
        CheckpointSpriteComponent->Play();
    }
    else
    {

        CheckpointSpriteComponent->SetFlipbook(UpFlipbook);
        CheckpointSpriteComponent->SetLooping(true);
        CheckpointSpriteComponent->Play();
    }
}

