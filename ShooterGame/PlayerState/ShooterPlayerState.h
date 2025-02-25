// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShooterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AShooterPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

	// Replication notifies
	virtual void OnRep_Score() override;

	UFUNCTION()
	virtual void OnRep_Deaths();

	void AddToScore(float ScoreAmount);
	void AddToDeaths(int32 DeathsAmount);


private:
	UPROPERTY()
	class AShooterCharacter* Character;

	UPROPERTY()
	class AShooterPlayerController* Controller;

	UPROPERTY(ReplicatedUsing = OnRep_Deaths)
	int32 Deaths;
	
};
