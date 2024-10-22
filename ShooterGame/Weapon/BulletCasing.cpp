// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletCasing.h"

ABulletCasing::ABulletCasing()
{
	PrimaryActorTick.bCanEverTick = false;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet Casing"));
	SetRootComponent(CasingMesh);

}

void ABulletCasing::BeginPlay()
{
	Super::BeginPlay();
	
}
