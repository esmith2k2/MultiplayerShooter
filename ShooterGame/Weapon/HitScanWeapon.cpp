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
        FVector End = Start + (HitTarget - Start) * 1.25f;

        FHitResult FireHit;
        UWorld* World = GetWorld();
        if(World)
        {
            World->LineTraceSingleByChannel(FireHit, Start, End, ECollisionChannel::ECC_Visibility);

            FVector BeamEnd = End;

            if(FireHit.bBlockingHit)
            {
                BeamEnd = FireHit.ImpactPoint;
                AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(FireHit.GetActor());
                if(ShooterCharacter && HasAuthority() && InstigatorController)
                {
                    UGameplayStatics::ApplyDamage(ShooterCharacter, Damage, InstigatorController, this, UDamageType::StaticClass());
                }
                if(ImpactParticles)
                {
                    UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticles, FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());
                }
                if(BodyHitSound && ShooterCharacter)
                {
                    UGameplayStatics::SpawnSoundAtLocation(this, BodyHitSound, FireHit.ImpactPoint);
                }
                else if(EnvironmentHitSound)
                {
                    UGameplayStatics::SpawnSoundAtLocation(this, EnvironmentHitSound, FireHit.ImpactPoint);
                }
            }
            if(BeamParticles)
            {
                UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(World, BeamParticles, SocketTransform);
                if(Beam)
                {
                    Beam->SetVectorParameter(FName("Target"), BeamEnd);
                }
            }
        }
        if(MuzzleFlash)
        {
            UGameplayStatics::SpawnEmitterAtLocation(World, MuzzleFlash, SocketTransform);
        }
        if(FireSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        }
    }

    Super::Fire(HitTarget);
}
