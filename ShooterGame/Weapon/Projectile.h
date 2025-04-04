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

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY()
	class USoundCue* ImpactSoundToPlay;

	UPROPERTY(EditAnywhere, Category = DamageValues)
	float Damage = 15.f;
	
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;
public:	




	UPROPERTY(EditAnywhere)
	class UParticleSystem* Tracer;

	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* TracerComponent;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSoundEnvironment;

	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSoundBody;

};
