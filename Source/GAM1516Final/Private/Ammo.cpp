// Fill out your copyright notice in the Description page of Project Settings.

#include "Ammo.h"
#include <Components/SphereComponent.h>
#include "PaperSpriteComponent.h"
#include "MyBlueprintFunctionLibrary.h"

// Sets default values
AAmmo::AAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    HitBox = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    HitBox->SetCollisionProfileName("Overlap All");

    UMyBlueprintFunctionLibrary::LockPhysicsTo2DAxis(HitBox);
    HitBox->GetBodyInstance()->bLockZTranslation = true;
    HitBox->GetBodyInstance()->bLockXTranslation = true;
    RootComponent = HitBox;

    AmmoSprite = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite");
    AmmoSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    AmmoSprite->SetCollisionProfileName("NoCollision");
    AmmoSprite->SetupAttachment(RootComponent);

    Tags.Add("Ammo");
}

// Called when the game starts or when spawned
void AAmmo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

