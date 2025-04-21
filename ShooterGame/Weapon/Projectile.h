// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class SHOOTERGAME_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayImpactSound(class USoundCue* ImpactSoundTo);
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void StartDestroyTimer();
	void DestroyTimerFinished();
	void SpawnTrailSystem();
	void ExplodeDamage();

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY()
	class USoundCue* ImpactSoundToPlay;

	UPROPERTY(EditAnywhere, Category = "Projectile Properties")
	float Damage = 15.f;
	
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Projectile Properties")
	class UNiagaraSystem* TrailSystem;

	UPROPERTY()
	class UNiagaraComponent* TrailSystemComponent;

	FTimerHandle DestroyTimer;

	UPROPERTY(EditAnywhere, Category = "Projectile Properties")
	float DestroyTime = 3.f;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProjectileMesh;


	/*
	* Explosion Damage Values
	*/
	UPROPERTY(EditAnywhere, Category = "Projectile Properties|Explosion Damage Values")
	float MinimumDamage = 20.f;

	UPROPERTY(EditAnywhere, Category = "Projectile Properties|Explosion Damage Values")
	float InnerExplosionRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "Projectile Properties|Explosion Damage Values")
	float OutterExplosionRadius = 600.f;

	UPROPERTY(EditAnywhere, Category = "Projectile Properties|Explosion Damage Values")
	float FalloffCurveExponent= 1.f;

public:	




	UPROPERTY(EditAnywhere, Category = "Projectile Properties")
	class UParticleSystem* Tracer;

	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* TracerComponent;

	UPROPERTY(EditAnywhere, Category = "Projectile Properties")
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, Category = "Projectile Properties")
	class USoundCue* ImpactSoundEnvironment;

	UPROPERTY(EditAnywhere, Category = "Projectile Properties")
	class USoundCue* ImpactSoundBody;

};
