// Fill out your copyright notice in the Description page of Project Settings.

#include "Explosion.h"
#include "Classes/PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
AExplosion::AExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    HitBox = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    HitBox->SetCollisionProfileName("NoCollision");

    RootComponent = HitBox;

    UPaperSpriteComponent* comp = Cast<UPaperSpriteComponent>(GetComponentByClass(UPaperSpriteComponent::StaticClass()));
    if (comp)
    {
        comp->DestroyComponent();
        comp->SetActive(false);
    }

    ExplosionSpriteComponent = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(APaperCharacter::SpriteComponentName);
    if (ExplosionSpriteComponent)
    {
        ExplosionSpriteComponent->SetupAttachment(RootComponent);
        ExplosionSpriteComponent->SetCollisionProfileName("NoCollision");
        ExplosionSpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        ExplosionSpriteComponent->SetGenerateOverlapEvents(false);
    }
    Tags.Add("Explosion");

    InitialLifeSpan = 1.f;
}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

