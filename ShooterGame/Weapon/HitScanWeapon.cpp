// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "ShooterGame/Character/ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"


void AHitScanWeapon::Fire(const FVector &HitTarget) 
{
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if(OwnerPawn == nullptr) return; 
    AController* InstigatorController = OwnerPawn->GetController();


    const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
    if(MuzzleFlashSocket)
    {
        FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

        FVector Start = SocketTransform.GetLocation();

        FHitResult FireHit;
        WeaponTraceHit(Start, HitTarget, FireHit);

        AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(FireHit.GetActor());
        if(ShooterCharacter && HasAuthority() && InstigatorController)
        {
            UGameplayStatics::ApplyDamage(ShooterCharacter, Damage, InstigatorController, this, UDamageType::StaticClass());
        }
        if(ImpactParticles)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());
        }
        if(BodyHitSound && ShooterCharacter)
        {
            UGameplayStatics::SpawnSoundAtLocation(this, BodyHitSound, FireHit.ImpactPoint);
        }
        else if(EnvironmentHitSound)
        {
            UGameplayStatics::SpawnSoundAtLocation(this, EnvironmentHitSound, FireHit.ImpactPoint);
        }

        
        if(MuzzleFlash)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
        }
        if(FireSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        }
    }

    Super::Fire(HitTarget);
}

void AHitScanWeapon::WeaponTraceHit(const FVector &TraceStart, const FVector &HitTarget, FHitResult &OutHit) 
{
   

    UWorld* World = GetWorld();
    if(World)
    {
        FVector End = bUseScatter ? TraceEndWithScatter(TraceStart, HitTarget) : TraceStart + (HitTarget - TraceStart) * 1.25;
        World->LineTraceSingleByChannel(OutHit, TraceStart, End, ECollisionChannel::ECC_Visibility);
        FVector BeamEnd = End;
        if(OutHit.bBlockingHit)
        {
            BeamEnd = OutHit.ImpactPoint;
        }
        if(BeamParticles)
        {
            UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(World, BeamParticles, TraceStart, FRotator::ZeroRotator, true);
            if(Beam)
            {
                Beam->SetVectorParameter(FName("Target"), BeamEnd);
            }
        }

    }
}
