// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ShooterGameState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AShooterGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

	void UpdateTopScore(class AShooterPlayerState* ScoringPlayer);

	float TopScore = 0.f;

	UPROPERTY(Replicated)
	TArray<class AShooterPlayerState*> TopScoringPlayers;

private:


};
