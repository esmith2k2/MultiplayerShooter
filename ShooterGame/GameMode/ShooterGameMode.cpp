// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameMode.h"
#include "ShooterGame/Character/ShooterCharacter.h"
#include "ShooterGame/PlayerController/ShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"


void AShooterGameMode::PlayerEliminated(class AShooterCharacter* EliminatedCharacter, class AShooterPlayerController* VictimController, AShooterPlayerController* AttackerController) 
{
    if(EliminatedCharacter)
    {
        EliminatedCharacter->Elim();
    }
}

void AShooterGameMode::RequestRespawn(class ACharacter* EliminatedCharacter, class AController* EliminatedController)
{
    if(EliminatedCharacter)
    {
        EliminatedCharacter->Reset();
        EliminatedCharacter->Destroy();
    }
    if(EliminatedController)
    {
        TArray<AActor*> PlayerStarts; 
        UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
        int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
        RestartPlayerAtPlayerStart(EliminatedController, PlayerStarts[Selection]);

    }
   
}
