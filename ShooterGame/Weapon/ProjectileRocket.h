// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileRocket.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AProjectileRocket : public AProjectile
{
	GENERATED_BODY()

public:

	AProjectileRocket();

	virtual void Destroyed() override;

protected:

	virtual void BeginPlay() override;

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	void DestroyTimerFinished();

	UPROPERTY(EditAnywhere, Category = DamageValues)
	float InnerExplosionRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = DamageValues)
	float OutterExplosionRadius = 600.f;

	UPROPERTY(EditAnywhere, Category = DamageValues)
	float MinimumDamage = 20.f;

	UPROPERTY(EditAnywhere, Category = DamageValues)
	float FalloffCurveExponent= 1.f;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* TrailSystem;

	UPROPERTY()
	class UNiagaraComponent* TrailSystemComponent;

	UPROPERTY(EditAnywhere)
	USoundCue* ProjectileLoop;

	UPROPERTY()
	UAudioComponent* ProjectileLoopComponent;

	UPROPERTY(EditAnywhere)
	USoundAttenuation* LoopingSoundAttenuation;

	UPROPERTY(VisibleAnywhere)
	class URocketMovementComponent* RocketMovementComponent;

private:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* RocketMesh;

	FTimerHandle DestroyTimer;

	UPROPERTY(EditAnywhere)
	float DestroyTime = 3.f;
	
};
