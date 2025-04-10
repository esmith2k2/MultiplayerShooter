// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameMode.h"
#include "ShooterGame/Character/ShooterCharacter.h"
#include "ShooterGame/PlayerController/ShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "ShooterGame/PlayerState/ShooterPlayerState.h"
#include "ShooterGame/GameState/ShooterGameState.h"

namespace MatchState
{
    const FName Cooldown = FName("Cooldown");
}


AShooterGameMode::AShooterGameMode() 
{
    bDelayedStart = true;
}

void AShooterGameMode::BeginPlay() 
{
    Super::BeginPlay();

    LevelStartingTime = GetWorld()->GetTimeSeconds();

}


void AShooterGameMode::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);

    if(MatchState == MatchState::WaitingToStart)
    {
        CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
        if(CountdownTime <= 0.f)
        {
            StartMatch();
        }
    }
    else if(MatchState == MatchState::InProgress)
    {
        CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
        if(CountdownTime <= 0.f)
        {
            SetMatchState(MatchState::Cooldown);
        }
    }
    else if(MatchState == MatchState::Cooldown)
    {
        CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
        if(CountdownTime <= 0.f)
        {
            RestartGame();
        }
    }


}

void AShooterGameMode::OnMatchStateSet() 
{
    Super::OnMatchStateSet();

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AShooterPlayerController* ShooterPlayerController = Cast<AShooterPlayerController>(*It);
        if(ShooterPlayerController)
        {
            ShooterPlayerController->OnMatchStateSet(MatchState);
        }
    }
}

void AShooterGameMode::PlayerEliminated(class AShooterCharacter* EliminatedCharacter, class AShooterPlayerController* VictimController, AShooterPlayerController* AttackerController) 
{

    AShooterPlayerState* AttackerPlayerState = AttackerController ? Cast<AShooterPlayerState>(AttackerController->PlayerState) : nullptr;
    AShooterPlayerState* VictimPlayerState = VictimController ? Cast<AShooterPlayerState>(VictimController->PlayerState) : nullptr;

    AShooterGameState* ShooterGameState = GetGameState<AShooterGameState>();

    if(AttackerPlayerState && AttackerPlayerState != VictimPlayerState && ShooterGameState)
    {
        AttackerPlayerState->AddToScore(1.f);
        ShooterGameState->UpdateTopScore(AttackerPlayerState);
    }
    if(VictimPlayerState)
    {
        VictimPlayerState->AddToDeaths(1);
    }

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




