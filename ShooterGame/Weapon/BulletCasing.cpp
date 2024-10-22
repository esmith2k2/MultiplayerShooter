// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletCasing.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ABulletCasing::ABulletCasing()
{
	PrimaryActorTick.bCanEverTick = false;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet Casing"));
	SetRootComponent(CasingMesh);
	CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	CasingMesh->SetNotifyRigidBodyCollision(true);
	ShellEjectionImpulse = 10.f;

}

void ABulletCasing::BeginPlay()
{
	Super::BeginPlay();

	CasingMesh->OnComponentHit.AddDynamic(this, &ABulletCasing::OnHit);
	CasingMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);

	SetLifeSpan(3.f);
	
}

void ABulletCasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) 
{
	if(ShellSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, ShellSound, GetActorLocation());
	}

	CasingMesh->SetNotifyRigidBodyCollision(false);
	
}
