// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterGame/HUD/ShooterHUD.h"
#include "ShooterGame/Weapon/WeaponTypes.h"
#include "ShooterGame/ShooterTypes/CombatState.h"
#include "CombatComponent.generated.h"

#define TRACE_LENGTH 100000.f

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTERGAME_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	friend class AShooterCharacter;	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
	
	void EquipWeapon(class AWeapon* WeaponToEquip);
	void Reload();
	
	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	void FireButtonPressed(bool bPressed);
	
protected:
	virtual void BeginPlay() override;

	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();



	void Fire();

	bool bFireButtonPressed;

	

	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void ServerReload();

	void HandleReload();

	int32 AmountToReload();

private:
	UPROPERTY()
	class AShooterCharacter* Character;
	UPROPERTY()
	class AShooterPlayerController* Controller;
	UPROPERTY()
	class AShooterHUD* HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;
	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	/*
	HUD and Crosshairs
	*/

	UPROPERTY(EditAnywhere)
	float CrosshairVelocityFactor;

	UPROPERTY(EditAnywhere)
	float CrosshairInAirFactor;

	UPROPERTY(EditAnywhere)
	float CrosshairAimingFactor;

	UPROPERTY(EditAnywhere)
	float CrosshairShootingFactor;

	FVector HitTarget;

	FVector AimEndLocation;

	FHUDPackage HUDPackage;


	/*
	 Aiming and FOV
	*/

	// Field of view when not aiming; set to camera's default FOV in BeginPlay
	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomedFOV = 30.f;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);

	/*
		Automatic Fire
	*/

	FTimerHandle FireTimer;

	bool bCanFire = true;

	bool CanFire();

	void StartFireTimer();
	void FireTimerFinished();

	// Ammo being carried for the currently-equipped weapon
	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	UFUNCTION()
	void OnRep_CarriedAmmo();

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
	int32 StartingAssaultRifleAmmo = 30;

	UPROPERTY(EditAnywhere)
	int32 StartingRocketAmmo = 1;

	UPROPERTY(EditAnywhere)
	int32 StartingPistolAmmo = 45;

	void InitializeCarriedAmmo();

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UFUNCTION()
	void OnRep_CombatState();

	void UpdateAmmoValues();


public:	

	FORCEINLINE FVector GetAimEndLocation() { return AimEndLocation; }

		
};
