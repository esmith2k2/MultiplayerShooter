// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ShooterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AShooterGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

	AShooterGameMode();

	virtual void Tick(float DeltaTime) override;

	virtual void PlayerEliminated(class AShooterCharacter* EliminatedCharacter, class AShooterPlayerController* VictimController, AShooterPlayerController* AttackerController);

	virtual void RequestRespawn(class ACharacter* EliminatedCharacter, class AController* EliminatedController);

	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 15.f;

	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;

	float LevelStartingTime = 0.f;

protected:

	virtual void BeginPlay() override;

	virtual void OnMatchStateSet() override;


private:

	float CountdownTime = 0.f;

};
