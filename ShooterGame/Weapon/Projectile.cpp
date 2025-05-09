// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "ShooterGame/Character/ShooterCharacter.h"
#include "ShooterGame/ShooterGame.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECollisionResponse::ECR_Block);


}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(Tracer, CollisionBox, FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition);
	}

	if(HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}
	
}



// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!OtherActor && OtherComp)
    {
        return;
    }

    // Cast to the target character
    AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
	
    if (ShooterCharacter)
    {
		ImpactSoundToPlay = ImpactSoundBody;
    }
	else
	{
		ImpactSoundToPlay = ImpactSoundEnvironment;
	}

	if(ImpactSoundToPlay)
	{
		MulticastPlayImpactSound(ImpactSoundToPlay);
	}

    // Destroy the projectile
    Destroy();
}

void AProjectile::SpawnTrailSystem() 
{
	if(TrailSystem)
    {
        TrailSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailSystem, GetRootComponent(), FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
    }
}

void AProjectile::ExplodeDamage() 
{
    APawn* FiringPawn = GetInstigator();
    if(FiringPawn && HasAuthority())
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
}

void AProjectile::MulticastPlayImpactSound_Implementation(USoundCue* ImpactSound) 
{
	if(ImpactSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}

void AProjectile::StartDestroyTimer() 
{
	 GetWorldTimerManager().SetTimer(DestroyTimer, this, &AProjectile::DestroyTimerFinished, DestroyTime);
}

void AProjectile::DestroyTimerFinished() 
{
	
    Destroy();
}

void AProjectile::Destroyed() 
{
	if(ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}
	
	Super::Destroyed();
}

