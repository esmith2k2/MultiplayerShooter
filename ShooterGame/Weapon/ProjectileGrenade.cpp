// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


AProjectileGrenade::AProjectileGrenade() 
{
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grenade Mesh"));
    ProjectileMesh->SetupAttachment(RootComponent);
    ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->SetIsReplicated(true);
    ProjectileMovementComponent->bShouldBounce = true;
}

void AProjectileGrenade::BeginPlay() 
{
    AActor::BeginPlay();
    
    StartDestroyTimer();
    SpawnTrailSystem();

    ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &AProjectileGrenade::OnBounce);

}

void AProjectileGrenade::OnBounce(const FHitResult &ImpactResult, const FVector &ImpactVelocity) 
{
    if(BounceSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, BounceSound, GetActorLocation());
    }
}

void AProjectileGrenade::Destroyed() 
{
    UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
    ExplodeDamage();

    Super::Destroyed();
}