// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlatform.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "MyBlueprintFunctionLibrary.h"

// Sets default values
AMyPlatform::AMyPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    HitBox = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
    HitBox->SetCollisionProfileName("BlockAll");
    HitBox->SetNotifyRigidBodyCollision(true);
    HitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    HitBox->SetSimulatePhysics(false);

    UMyBlueprintFunctionLibrary::LockPhysicsTo2DAxis(HitBox);
    HitBox->GetBodyInstance()->bLockXTranslation = true;
    HitBox->GetBodyInstance()->bLockZTranslation = true;

    RootComponent = HitBox;

    PlatformSprite = CreateDefaultSubobject<UPaperSpriteComponent>("Platform");
    PlatformSprite->SetCollisionProfileName("NoCollision");
    PlatformSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PlatformSprite->SetGenerateOverlapEvents(false);
    PlatformSprite->SetupAttachment(RootComponent);

    Tags.Add("GameObject");
}

// Called when the game starts or when spawned
void AMyPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

