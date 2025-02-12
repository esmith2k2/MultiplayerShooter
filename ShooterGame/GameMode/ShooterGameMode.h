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

	virtual void PlayerEliminated(class AShooterCharacter* EliminatedCharacter, class AShooterPlayerController* VictimController, AShooterPlayerController* AttackerController);

	virtual void RequestRespawn(class ACharacter* EliminatedCharacter, class AController* EliminatedController);
};
