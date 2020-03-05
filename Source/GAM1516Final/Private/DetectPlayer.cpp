// Fill out your copyright notice in the Description page of Project Settings.

#include "DetectPlayer.h"
#include "Components/BoxComponent.h"

// Sets default values for this component's properties
UDetectPlayer::UDetectPlayer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
    DetectPlayer = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
    DetectPlayer->SetCollisionProfileName("Overlap All");
    DetectPlayer->SetNotifyRigidBodyCollision(true);
    DetectPlayer->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    DetectPlayer->SetSimulatePhysics(false);
}


// Called when the game starts
void UDetectPlayer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDetectPlayer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

