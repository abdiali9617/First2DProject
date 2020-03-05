// Fill out your copyright notice in the Description page of Project Settings.

#include "GunInventoryComponent.h"
#include "Gun.h"
#include "Arm.h"

// Sets default values for this component's properties
UGunInventoryComponent::UGunInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGunInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
    Arm = Cast<AArm>(GetOwner());
    check(Arm != nullptr && "UGunInventoryComponent::BeginPlay - Arm is Null");
}


// Called every frame
void UGunInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGunInventoryComponent::AddToGunInventory(AGun * gun)
{
    GunArray.Add(gun);
}

void UGunInventoryComponent::ResetCurrentInventory()
{
    CurrentGun = nullptr;
}

int UGunInventoryComponent::GetGunCount()
{
    return GunArray.Num();
}

void UGunInventoryComponent::NextGun()
{
    Iterator++;
    if (Iterator >= GunArray.Num())
    {
        Iterator = 0;
    }
    CurrentGun = GunArray[Iterator];
    Cast<AGun>(CurrentGun)->Equip(Cast<AArm>(GetOwner()));
}

void UGunInventoryComponent::PreviousGun()
{
    Iterator--;
    if (Iterator < 0)
    {
        Iterator = GunArray.Num() - 1;
    }
    CurrentGun = GunArray[Iterator];
    Cast<AGun>(CurrentGun)->Equip(Cast<AArm>(GetOwner()));
}

AGun * UGunInventoryComponent::GetCurrentGun() const
{
    return CurrentGun;
}

