// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "ShooterGame/Character/ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"


void AHitScanWeapon::Fire(const FVector &HitTarget) 
{
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if(OwnerPawn == nullptr) return; 
    AController* InstigatorController = OwnerPawn->GetController();

    const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
    if(MuzzleFlashSocket && InstigatorController)
    {
        FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

        FVector Start = SocketTransform.GetLocation();
        FVector End = Start + (HitTarget - Start) * 1.25f;

        FHitResult FireHit;
        UWorld* World = GetWorld();
        if(World)
        {
            World->LineTraceSingleByChannel(FireHit, Start, End, ECollisionChannel::ECC_Visibility);
            if(FireHit.bBlockingHit)
            {
                AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(FireHit.GetActor());
                if(ShooterCharacter)
                {
                    if(HasAuthority())
                    {
                        UGameplayStatics::ApplyDamage(ShooterCharacter, Damage, InstigatorController, this, UDamageType::StaticClass());
                    }
                }
                if(ImpactParticles)
                {
                    UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticles, HitTarget, FireHit.ImpactNormal.Rotation());
                }
            }
        }
    }

    Super::Fire(HitTarget);
}
