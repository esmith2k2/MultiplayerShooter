// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitScanWeapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AShotgun : public AHitScanWeapon
{
	GENERATED_BODY()

public: 

	virtual void Fire(const FVector &HitTarget) override;

protected:


private:

	UPROPERTY(EditAnywhere, Category = "ShotGun Attributes")
	uint32 NumberOfPellets = 20;

	
};
