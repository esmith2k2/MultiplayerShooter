// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"
#include "Engine/SkeletalMeshSocket.h"
#include "ShooterGame/Character/ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"





void AShotgun::Fire(const FVector &HitTarget) 
{
    AWeapon::Fire(HitTarget);

    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if(OwnerPawn == nullptr) return; 
    AController* InstigatorController = OwnerPawn->GetController();


    const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
    if(MuzzleFlashSocket)
    {
        FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

        FVector Start = SocketTransform.GetLocation();

        TMap<AShooterCharacter*, uint32> HitMap;
        for(uint32 i = 0; i < NumberOfPellets; i++)
        {
            FHitResult FireHit;
            WeaponTraceHit(Start, HitTarget, FireHit);

            AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(FireHit.GetActor());
            if(ShooterCharacter && HasAuthority() && InstigatorController)
            {
                if(HitMap.Contains(ShooterCharacter))
                {
                    HitMap[ShooterCharacter]++;
                }
                else
                {
                    HitMap.Emplace(ShooterCharacter, 1);
                }
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
        }
        
        for (auto HitPair : HitMap)
        {
            if(HitPair.Key && HasAuthority() && InstigatorController)
            {
                UGameplayStatics::ApplyDamage(HitPair.Key, Damage * HitPair.Value, InstigatorController, this, UDamageType::StaticClass());
            }
        }

    }

}
