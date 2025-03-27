// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileRocket.h"
#include "Kismet/GameplayStatics.h"

AProjectileRocket::AProjectileRocket() 
{
    RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket Mesh"));
    RocketMesh->SetupAttachment(RootComponent);
    RocketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



void AProjectileRocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) 
{

    APawn* FiringPawn = GetInstigator();
    if(FiringPawn)
    {
        AController* FiringController = FiringPawn->GetController();
        {
            if(FiringController)
            {
                UGameplayStatics::ApplyRadialDamageWithFalloff(
                    this, // World Context Object
                    Damage, // Base Damage
                    MinimumDamage,  // Minimum Damage
                    GetActorLocation(), // Origin
                    InnerExplosionRadius, // Damage Inner Radius
                    OutterExplosionRadius, // Damage Outter Radius
                    FalloffCurveExponent, // Damage Falloff
                    UDamageType::StaticClass(), // Damage Type Class
                    TArray<AActor*>(), // Ignore Actors Array
                    this, // Damage Causer
                    FiringController // Instigator Controller
                );
            }
        }
    }


    Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}


