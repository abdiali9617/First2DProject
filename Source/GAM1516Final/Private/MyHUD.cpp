// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "ConstructorHelpers.h"

AMyHUD::AMyHUD()
{
    static ConstructorHelpers::FClassFinder<UUserWidget>
        playerHud(TEXT("/Game/Blueprints/UI/MyHUD"));
    if (playerHud.Succeeded())
        PlayerGuiClass = playerHud.Class;
}

void AMyHUD::BeginPlay()
{
    PlayerGui = CreateWidget<UUserWidget>(GetGameInstance(),
        PlayerGuiClass);
    PlayerGui->AddToViewport();
}
