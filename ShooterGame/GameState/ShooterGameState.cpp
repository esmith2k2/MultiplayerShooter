// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameState.h"
#include "Net/UnrealNetwork.h"
#include "ShooterGame/PlayerState/ShooterPlayerState.h"


void AShooterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const 
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AShooterGameState, TopScoringPlayers);
}

void AShooterGameState::UpdateTopScore(class AShooterPlayerState* ScoringPlayer) 
{
    if (TopScoringPlayers.Num() == 0)
    {
        TopScoringPlayers.AddUnique(ScoringPlayer);
        TopScore = ScoringPlayer->GetScore();
    }
    else if (ScoringPlayer->GetScore() == TopScore)
    {
        TopScoringPlayers.AddUnique(ScoringPlayer);
    }
    else if (ScoringPlayer->GetScore() > TopScore)
    {
        TopScoringPlayers.Empty();
        TopScoringPlayers.AddUnique(ScoringPlayer);
        TopScore = ScoringPlayer->GetScore();
    }
}
