// Fill out your copyright notice in the Description page of Project Settings.

#include "Arm.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "Classes/PaperSpriteComponent.h"
#include "MyBlueprintFunctionLibrary.h"
#include "GunInventoryComponent.h"
#include "Gun.h"
#include "MyGameStateBase.h"
#include "Engine/Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

// Sets default values
AArm::AArm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    HitBox = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
    HitBox->SetCollisionProfileName("NoCollision");
    HitBox->SetNotifyRigidBodyCollision(true);
    HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    HitBox->SetGenerateOverlapEvents(true);
    HitBox->SetSimulatePhysics(false);

    HitBox->GetBodyInstance()->bLockYRotation = true;
    HitBox->GetBodyInstance()->bLockYTranslation = true;
    HitBox->GetBodyInstance()->bLockXTranslation = true;
    HitBox->GetBodyInstance()->bLockZTranslation = true;

    RootComponent = HitBox;

    ArmSprite = CreateDefaultSubobject<UPaperSpriteComponent>("Actor Visual");
    ArmSprite->SetCollisionProfileName("NoCollision");
    ArmSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ArmSprite->SetGenerateOverlapEvents(false);
    ArmSprite->SetupAttachment(RootComponent);

    PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
    PickupSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    PickupSphere->SetCollisionProfileName("OverlapAll");
    PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &AArm::OnBeginOverlap);
    PickupSphere->SetupAttachment(RootComponent);

    GunInventoryComponent = CreateDefaultSubobject<UGunInventoryComponent>("GunInventory");

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEmitter"));
    AudioComponent->bAutoActivate = false;
    AudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    AudioComponent->SetupAttachment(RootComponent);

    Tags.Add("Arm");
}

// Called when the game starts or when spawned
void AArm::BeginPlay()
{
	Super::BeginPlay();

    DefaultGun = Cast<AGun>(GetWorld()->SpawnActor(DefaultGunRef));
    GunInventoryComponent->AddToGunInventory(Cast<AGun>(DefaultGun));
    CurrentGun = Cast<AGun>(DefaultGun);
}

// Called every frame
void AArm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArm::NextGun()
{
    if (GunInventoryComponent->GetCurrentGun() != nullptr)
    {
        Cast<AGun>(GunInventoryComponent->GetCurrentGun())->UnEquip();
    }

    GunInventoryComponent->NextGun();
    CurrentGun = Cast<AGun>(GunInventoryComponent->GetCurrentGun());
}

void AArm::PrevGun()
{
    if (GunInventoryComponent->GetCurrentGun() != nullptr)
    {
        Cast<AGun>(GunInventoryComponent->GetCurrentGun())->UnEquip();
    }

    GunInventoryComponent->PreviousGun();
    CurrentGun = Cast<AGun>(GunInventoryComponent->GetCurrentGun());
}

void AArm::PickupGun()
{
    TArray<AActor*> overlappingActors;
    PickupSphere->GetOverlappingActors(overlappingActors);
    for (auto& actor : overlappingActors)
    {
        if (Cast<AGun>(actor))
        {
            if (GunInventoryComponent != nullptr)
            {
                GunInventoryComponent->AddToGunInventory(Cast<AGun>(actor));
                Cast<AGun>(actor)->Disable();
            }
        }
    }
}

void AArm::FireGun()
{
    if (CurrentGun != Cast<AGun>(DefaultGun))
        CurrentGun->Fire();
}

void AArm::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor != nullptr)
    {
        if (OtherActor->ActorHasTag(FName("Ammo")))
        {
            if (CurrentGun != Cast<AGun>(DefaultGun))
            {
                if (CurrentGun->CurrentAmmo <= CurrentGun->MaximumAmmo - 10)
                {
                    OtherActor->Destroy();
                    AudioComponent->SetSound(AmmoPick);
                    AudioComponent->Play();
                    CurrentGun->CurrentAmmo += 10;
                    AMyGameStateBase* GameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
                    GameState->SetAmmo(CurrentGun->CurrentAmmo);
                }
            }
        }
    }
}
