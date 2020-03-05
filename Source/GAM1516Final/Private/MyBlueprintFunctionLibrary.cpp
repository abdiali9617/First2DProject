// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBlueprintFunctionLibrary.h"
#include "Components/PrimitiveComponent.h"

void UMyBlueprintFunctionLibrary::LockPhysicsTo2DAxis(UPrimitiveComponent * primitive)
{
    primitive->GetBodyInstance()->bLockRotation = true;
    primitive->GetBodyInstance()->bLockXRotation = true;
    primitive->GetBodyInstance()->bLockYRotation = true;
    primitive->GetBodyInstance()->bLockZRotation = true;
    primitive->GetBodyInstance()->bLockYTranslation = true;
}
