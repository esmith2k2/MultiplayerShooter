// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ShooterGame/Weapon/Weapon.h"
#include "ShooterGame/ShooterComponents/CombatComponent.h"


void UShooterAnimInstance::NativeInitializeAnimation() 
{
    Super::NativeInitializeAnimation();

    ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());

}

void UShooterAnimInstance::NativeUpdateAnimation(float DeltaTime) 
{
    Super::NativeUpdateAnimation(DeltaTime);

    if(ShooterCharacter == nullptr)
    {
        ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
    }

    if(ShooterCharacter == nullptr) return;

    FVector Velocity = ShooterCharacter->GetVelocity();
    Velocity.Z = 0.f;
    Speed = Velocity.Size();

    bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();

    bIsAccelerating = ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

    bWeaponEquipped = ShooterCharacter->IsWeaponEquipped();

    EquippedWeapon = ShooterCharacter->GetEquippedWeapon();

    bIsCrouched = ShooterCharacter->bIsCrouched;

    bAiming = ShooterCharacter->IsAiming();

    TurningInPlace = ShooterCharacter->GetTurningInPlace();

    bRotateRootBone = ShooterCharacter->ShouldRotateRootBone();

    //Offset yaw for strafing
    FRotator AimRotation = ShooterCharacter->GetBaseAimRotation();
    FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity());
    FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
    DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, GetDeltaSeconds(), 6.f);
    YawOffset = DeltaRotation.Yaw;

    //Lean
    CharacterRotationLastFrame = CharacterRotation;
    CharacterRotation = ShooterCharacter->GetActorRotation();
    const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
    const float Target = Delta.Yaw / GetDeltaSeconds();
    const float Interp = FMath::FInterpTo(Lean, Target, GetDeltaSeconds(), 2.f);
    Lean = FMath::Clamp(Interp, -30.f, 30.f);
    
    AO_Yaw = ShooterCharacter->GetAO_Yaw();
    AO_Pitch = ShooterCharacter->GEtAO_Pitch();

    if(bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && ShooterCharacter->GetMesh())
    {
        LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
        FVector OutPosition;
        FRotator OutRotation;

        ShooterCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
        LeftHandTransform.SetLocation(OutPosition);
        LeftHandTransform.SetRotation(FQuat(OutRotation));


        if(ShooterCharacter->IsLocallyControlled())
        {
            bLocallyControlled = true;
            FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("Hand_R"), ERelativeTransformSpace::RTS_World);
            FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), (RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - ShooterCharacter->GetHitTarget())));
            RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaTime, 25.f);
        }
        
        /*
        Debug lines for weapon rotation to aim location

        FTransform MuzzleTipTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("MuzzleFlash"), ERelativeTransformSpace::RTS_World);
        FVector MuzzleX(FRotationMatrix(MuzzleTipTransform.GetRotation().Rotator()).GetUnitAxis(EAxis::X));
        DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(),MuzzleTipTransform.GetLocation() + MuzzleX * 1000, FColor::Red, false, -1.f, 0U, 1.f);
        DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), ShooterCharacter->GetHitTarget(), FColor::Blue, false, -1.f, 0U, 1.f);
        */
       
        
        

    }

   
    
}
