// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "MyPlayer.h"

void AMyPlayerController::OnPossess(APawn * aPawn)
{
    Super::Possess(aPawn);
}

void AMyPlayerController::OnUnPossess()
{
    APawn* posessedPawn = Cast<APawn>(GetOwner());
    Super::UnPossess();
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (InputComponent != nullptr)
    {
       InputComponent->BindAxis("MoveRight", this, &AMyPlayerController::MoveRight);
       InputComponent->BindAction("Jump", IE_Pressed, this, &AMyPlayerController::Jump);

       InputComponent->BindAction("NextGun", IE_Pressed, this, &AMyPlayerController::NextGun);
       InputComponent->BindAction("PrevGun", IE_Pressed, this, &AMyPlayerController::PrevGun);
       InputComponent->BindAction("PickupGun", IE_Pressed, this, &AMyPlayerController::PickupGun);
       InputComponent->BindAction("FireGun", IE_Pressed, this, &AMyPlayerController::FireGun);
    }
}

void AMyPlayerController::MoveRight(float value)
{
    AMyPlayer* pawn = Cast<AMyPlayer>(GetPawn());
    if (pawn)
        pawn->MoveRight(value);
}

void AMyPlayerController::NextGun()
{
    AMyPlayer* pawn = Cast<AMyPlayer>(GetPawn());
    if (pawn)
        pawn->NextGun();
}

void AMyPlayerController::PrevGun()
{
    AMyPlayer* pawn = Cast<AMyPlayer>(GetPawn());
    if (pawn)
        pawn->PrevGun();
}

void AMyPlayerController::PickupGun()
{
    AMyPlayer* pawn = Cast<AMyPlayer>(GetPawn());
    if (pawn)
        pawn->PickupGun();
}

void AMyPlayerController::FireGun()
{
    AMyPlayer* pawn = Cast<AMyPlayer>(GetPawn());
    if (pawn)
        pawn->FireGun();
}

void AMyPlayerController::Jump()
{
    AMyPlayer* pawn = Cast<AMyPlayer>(GetPawn());
    if (pawn)
        pawn->Jump();
}
