// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class AExplosion;
UCLASS()
class GAM1516FINAL_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
        UParticleSystem* ProjectileFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
        int TopSpeed = 20000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
        int StartSpeed = 5000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
        int Damage = 10;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
        class USoundBase* ImpactSound;

    UPROPERTY(EditAnywhere, Category = Attachable)
        TSubclassOf<AExplosion> Explosion;

    AExplosion* m_Explosion;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Bullet Visual")
        class UPaperSpriteComponent* BulletSprite;

    UPROPERTY(Category = Collision, VisibleAnywhere,
        meta = (AllowPrivateAccess))
        class USphereComponent* HitBox;

    UPROPERTY(Category = Projectile, VisibleAnywhere,
        meta = (AllowPrivateAccess))
        class UProjectileMovementComponent* ProjectileMovementComponent;

    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    void SetFireDirection(FVector dir);
	
};
