// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/SphereComponent.h>
#include <Components/ArrowComponent.h>
#include "PaperSpriteComponent.h"
#include "Components/AudioComponent.h"
#include "Arm.h"
#include "Bullet.h"
#include "MyBlueprintFunctionLibrary.h"
#include "MyGameStateBase.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    HitBox = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    HitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    HitBox->SetSimulatePhysics(false);
    HitBox->SetCollisionProfileName("OverlapAll");
    HitBox->SetGenerateOverlapEvents(true);

    UMyBlueprintFunctionLibrary::LockPhysicsTo2DAxis(HitBox);
    RootComponent = HitBox;

    GunSprite = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite");
    GunSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GunSprite->SetCollisionProfileName("NoCollision");
    GunSprite->SetupAttachment(RootComponent);

    SpawnPoint = CreateDefaultSubobject<UArrowComponent>("SpawnPoint");
    SpawnPoint->SetupAttachment(RootComponent);

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEmitter"));
    AudioComponent->bAutoActivate = false;
    AudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    AudioComponent->SetupAttachment(RootComponent);

    Tags.Add("Gun");

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::Disable()
{
    SetActorHiddenInGame(true);

    SetActorEnableCollision(false);
}

void AGun::Enable()
{
    SetActorHiddenInGame(false);

    SetActorEnableCollision(true);
}

void AGun::Fire()
{
    if (!(CurrentAmmo == 0))
    {
        SpawnBullet();
        CurrentAmmo--;
        AudioComponent->SetSound(FireSound);
        AudioComponent->Play();
        AMyGameStateBase* GameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
        GameState->SetAmmo(CurrentAmmo);
    }
}

FVector AGun::GetSpawnLocation() const
{
    return SpawnPoint->GetComponentToWorld().GetLocation();
}

void AGun::Equip(AArm* Arm)
{
    Enable();
    this->AttachToComponent(Arm->ArmSprite, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Hand");
    AMyGameStateBase* GameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
    GameState->SetAmmo(CurrentAmmo);
}

void AGun::UnEquip()
{
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    Disable();
}

void AGun::SpawnBullet()
{
    if (Bullet != nullptr)
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
            ABullet* SpawnedActor =
                World->SpawnActor<ABullet>(Bullet,
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

