// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponTypes.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped State"),
	EWS_Dropped UMETA(DisplayName = "Dropped State"),

	EWS_MAX UMETA(DisplayName = "Default State"),
};


UCLASS()
class SHOOTERGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	void ShowPickupWidget(bool bShowWidget);
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
	virtual void OnRep_Owner() override;
	void SetHUDAmmo();
	virtual void Fire(const FVector &HitTarget);
	void Dropped();
	void AddAmmo(int32 AmmoToAdd);


	// Textures for Crosshairs
	UPROPERTY(EditAnywhere, Category = "Default Weapon Parameters|Crosshairs")
	class UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = "Default Weapon Parameters|Crosshairs")
	class UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = "Default Weapon Parameters|Crosshairs")
	class UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = "Default Weapon Parameters|Crosshairs")
	class UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = "Default Weapon Parameters|Crosshairs")
	class UTexture2D* CrosshairsBottom;


	/* 
	 Zoomed FOV while aiming
	*/

	UPROPERTY(EditAnywhere, Category = "Default Weapon Parameters|Aiming")
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere, Category = "Default Weapon Parameters|Aiming")
	float ZoomInterpSpeed = 20.f;

	UPROPERTY(EditAnywhere, Category = "Default Weapon Parameters|Aiming")
	float UnZoomInterpSpeed = 20.f;
	
	/*
	* Automatic Fire
	*/

	UPROPERTY(EditAnywhere, Category = "Default Weapon Parameters|Firing")
	float FireDelay = .1f;

	UPROPERTY(EditAnywhere, Category = "Default Weapon Parameters|Firing")
	bool bAutomatic = true;

	UPROPERTY(EditAnywhere, Category = "Default Weapon Parameters|Firing")
	class USoundCue* EquipSound;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex
	);

	FVector TraceEndWithScatter(const FVector &TraceStart, const FVector &HitTarget);

private:
	UPROPERTY(VisibleAnywhere, Category = "Default Weapon Parameters|Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Default Weapon Parameters|Components")
	class USphereComponent* AreaSphere;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Default Weapon Parameters|Components")
	EWeaponState WeaponState;

	UFUNCTION()
	void OnRep_WeaponState();

	UPROPERTY(VisibleAnywhere, Category = "Default Weapon Parameters|Components")
	class UWidgetComponent* PickupWidget;

	UPROPERTY(EditAnywhere, Category = "Default Weapon Parameters|Firing")
	class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere, Category = "Default Weapon Parameters|Firing")
	TSubclassOf<class ABulletCasing> BulletCasingClass;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Ammo, Category = "Default Weapon Parameters|Ammo")
	int32 Ammo;

	UFUNCTION()
	void OnRep_Ammo();

	void SpendRound();

	UPROPERTY(EditAnywhere, Category = "Default Weapon Parameters|Ammo")
	int32 MagCapacity;

	UPROPERTY()
	class AShooterCharacter* ShooterOwnerCharacter;

	UPROPERTY()
	class AShooterPlayerController* ShooterOwnerController;

	UPROPERTY(EditAnywhere, Category = "Default Weapon Parameters|Ammo")
	EWeaponType WeaponType;

	/*
	* Trace end with scatter
	*/

	UPROPERTY(EditAnywhere, Category = "Default WeaponParameters|Scatter")
	float DistanceToSphere = 800.f;

	UPROPERTY(EditAnywhere, Category = "Default WeaponParameters|Scatter")
	float SphereRadius = 75.f;

	UPROPERTY(EditAnywhere, Category = "Default WeaponParameters|Scatter")
	bool bUseScatter = false;

public:	
	void SetWeaponState(EWeaponState State);
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }
	FORCEINLINE float GetUnZoomInterpSpeed() const { return UnZoomInterpSpeed; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMagCapacity() const { return MagCapacity; }
	bool IsEmpty();

};
