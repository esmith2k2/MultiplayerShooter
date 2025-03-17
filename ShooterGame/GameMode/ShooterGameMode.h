// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ShooterGameMode.generated.h"

namespace MatchState
{
	extern SHOOTERGAME_API const FName Cooldown; // Match duration has been reached. Display winner and begin cooldown timer. 
}

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

	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 15.f;

	float LevelStartingTime = 0.f;

protected:

	virtual void BeginPlay() override;

	virtual void OnMatchStateSet() override;


private:

	float CountdownTime = 0.f;

};
