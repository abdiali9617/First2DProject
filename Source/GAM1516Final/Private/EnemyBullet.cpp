// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBullet.h"
#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MyBlueprintFunctionLibrary.h"
#include "ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Explosion.h"
#include "MyGameStateBase.h"
#include "Engine/Engine.h"

// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    HitBox = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    HitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    HitBox->SetCollisionProfileName("BlockAll");
    HitBox->SetNotifyRigidBodyCollision(true);
    HitBox->SetSimulatePhysics(false);
    HitBox->SetEnableGravity(false);
    HitBox->OnComponentHit.AddDynamic(this, &AEnemyBullet::OnHit);
    UMyBlueprintFunctionLibrary::LockPhysicsTo2DAxis(HitBox);
    HitBox->GetBodyInstance()->bLockXTranslation = true;
    RootComponent = HitBox;

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
    ProjectileMovementComponent->SetUpdatedComponent(HitBox);
    ProjectileMovementComponent->InitialSpeed = StartSpeed;
    ProjectileMovementComponent->MaxSpeed = TopSpeed;
    ProjectileMovementComponent->ProjectileGravityScale = 0.f;

    ProjectileMovementComponent->bRotationFollowsVelocity = false;
    ProjectileMovementComponent->bShouldBounce = true;

    BulletSprite = CreateDefaultSubobject<UPaperSpriteComponent>("Actor Visual");
    BulletSprite->SetCollisionProfileName("NoCollision");
    BulletSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BulletSprite->SetGenerateOverlapEvents(false);
    BulletSprite->SetupAttachment(RootComponent);

    InitialLifeSpan = 5.f;

    Tags.Add("EnemyBullet");
}

// Called when the game starts or when spawned
void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBullet::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
    if (OtherActor != nullptr)
    {
        if (OtherActor->ActorHasTag(FName("GameObject")))
        {
            if (Explosion != nullptr)
            {
                m_Explosion = Cast<AExplosion>(GetWorld()->SpawnActor(Explosion));
                m_Explosion->SetActorLocation(this->GetActorLocation());
            }
            UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
            this->Destroy();
        }
        if (OtherActor->ActorHasTag(FName("Player")))
        {
            AMyGameStateBase* GameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
            int Health = GameState->GetPlayerHealth();
            Health -= 10;
            GameState->SetPlayerHealth(Health);
            if (Explosion != nullptr)
            {
                m_Explosion = Cast<AExplosion>(GetWorld()->SpawnActor(Explosion));
                m_Explosion->SetActorLocation(this->GetActorLocation());
            }
            UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
            this->Destroy();
        }
        if (OtherActor->ActorHasTag(FName("Enemy")))
        {
            if (Explosion != nullptr)
            {
                m_Explosion = Cast<AExplosion>(GetWorld()->SpawnActor(Explosion));
                m_Explosion->SetActorLocation(this->GetActorLocation());
            }
            UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
            this->Destroy();
        }
        if (OtherActor->ActorHasTag(FName("Tilan")))
        {
            if (Explosion != nullptr)
            {
                m_Explosion = Cast<AExplosion>(GetWorld()->SpawnActor(Explosion));
                m_Explosion->SetActorLocation(this->GetActorLocation());
            }
            UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
            this->Destroy();
        }
    }
}

void AEnemyBullet::SetFireDirection(FVector dir)
{
    ProjectileMovementComponent->Velocity = StartSpeed * dir;
}

