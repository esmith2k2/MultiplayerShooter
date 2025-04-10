// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitScanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()

public:

	virtual void Fire(const FVector &HitTarget) override;

protected:



private:

	UPROPERTY(EditAnywhere, Category = "HitScan Weapon Parameters")
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, Category = "HitScan Weapon Parameters")
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, Category = "HitScan Weapon Parameters")
	class UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere, Category = "HitScan Weapon Parameters")
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "HitScan Weapon Parameters")
	class USoundCue* FireSound;

	UPROPERTY(EditAnywhere, Category = "HitScan Weapon Parameters")
	class USoundCue* BodyHitSound;

	UPROPERTY(EditAnywhere, Category = "HitScan Weapon Parameters")
	class USoundCue* EnvironmentHitSound;
	
	
};
