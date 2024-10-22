// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletCasing.generated.h"

UCLASS()
class SHOOTERGAME_API ABulletCasing : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletCasing();

protected:

	virtual void BeginPlay() override;

private: 

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CasingMesh;


};
