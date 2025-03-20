// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "ShooterGame/HUD/ShooterHUD.h"
#include "ShooterGame/HUD/CharacterOverlay.h"
#include "ShooterGame/HUD/Announcement.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "ShooterGame/Character/ShooterCharacter.h"
#include "Net/UnrealNetwork.h"
#include "ShooterGame/GameMode/ShooterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterGame/ShooterComponents/CombatComponent.h"
#include "ShooterGame/GameState/ShooterGameState.h"
#include "ShooterGame/PlayerState/ShooterPlayerState.h"


void AShooterPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const 
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AShooterPlayerController, MatchState);
}

void AShooterPlayerController::BeginPlay() 
{
    Super::BeginPlay();

    ShooterHUD = Cast<AShooterHUD>(GetHUD());
    ServerCheckMatchState();

}

void AShooterPlayerController::PollInit() 
{
    if(CharacterOverlay == nullptr)
    {
        if(ShooterHUD && ShooterHUD->CharacterOverlay)
        {
            CharacterOverlay = ShooterHUD->CharacterOverlay;
            if(CharacterOverlay)
            {
                SetHUDHealth(HUDHealth, HUDMaxHealth);
                SetHUDScore(HUDScore);
                SetHUDDeaths(HUDDeaths);
            }
        }
    }
}

void AShooterPlayerController::ServerCheckMatchState_Implementation() 
{
    AShooterGameMode* GameMode = Cast<AShooterGameMode>(UGameplayStatics::GetGameMode(this));
    if(GameMode)
    {
        WarmupTime = GameMode->WarmupTime;
        MatchTime = GameMode->MatchTime;
        CooldownTime = GameMode->CooldownTime;
        LevelStartingTime = GameMode->LevelStartingTime;
        MatchState = GameMode->GetMatchState();
        ClientJoinMidgame(MatchState, WarmupTime, MatchTime, CooldownTime, LevelStartingTime);
    
    }
}

void AShooterPlayerController::ClientJoinMidgame_Implementation(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime) 
{
    WarmupTime = Warmup;
    MatchTime = Match;
    CooldownTime = Cooldown;
    LevelStartingTime = StartingTime;
    MatchState = StateOfMatch;
    OnMatchStateSet(MatchState);
    if(ShooterHUD && MatchState == MatchState::WaitingToStart)
    {
        ShooterHUD->AddAnnouncement();
    }
}

void AShooterPlayerController::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);

    SetHUDTime();
    CheckTimeSync(DeltaTime);
    PollInit();
}

void AShooterPlayerController::CheckTimeSync(float DeltaTime) 
{
    TimeSyncRunningTime += DeltaTime;
    if(IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
    {
        ServerRequestServerTime(GetWorld()->GetTimeSeconds());
        TimeSyncRunningTime = 0.f;
    }
}


void AShooterPlayerController::OnPossess(APawn* InPawn) 
{
    Super::OnPossess(InPawn);

    AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(InPawn);
    if(ShooterCharacter)
    {
        SetHUDHealth(ShooterCharacter->GetHealth(), ShooterCharacter->GetMaxHealth());
    }
}



void AShooterPlayerController::SetHUDHealth(float Health, float MaxHealth) 
{
    ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;

    bool bHUDValid = 
        ShooterHUD && 
        ShooterHUD->CharacterOverlay && 
        ShooterHUD->CharacterOverlay->HealthBar &&  
        ShooterHUD->CharacterOverlay->HealthText;

    if(bHUDValid)
    {
        const float HealthPercent = Health / MaxHealth;
        ShooterHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);

        FString HealthText = FString::Printf(TEXT("%d / %d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
        ShooterHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));

    }
    else
    {
        bInitializeCharacterOverlay = true;
        HUDHealth = Health;
        HUDMaxHealth = MaxHealth;
    }

}

void AShooterPlayerController::SetHUDScore(float Score) 
{
    ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;

    bool bHUDValid = 
        ShooterHUD && 
        ShooterHUD->CharacterOverlay && 
        ShooterHUD->CharacterOverlay->ScoreAmount;

    if(bHUDValid)
    {

        FString ScoreText = FString::Printf(TEXT("%d"), FMath::CeilToInt(Score));
        ShooterHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));

    }
    else
    {
        bInitializeCharacterOverlay = true;
        HUDScore = Score;
    }
}

void AShooterPlayerController::SetHUDDeaths(int32 Deaths) 
{
    ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;

    bool bHUDValid = 
        ShooterHUD && 
        ShooterHUD->CharacterOverlay && 
        ShooterHUD->CharacterOverlay->DeathsAmount;

    if(bHUDValid)
    {

        FString DeathsText = FString::Printf(TEXT("%d"), Deaths);
        ShooterHUD->CharacterOverlay->DeathsAmount->SetText(FText::FromString(DeathsText));

    }
    else
    {
        bInitializeCharacterOverlay = true;
        HUDDeaths = Deaths;
    }
}

void AShooterPlayerController::SetHUDWeaponAmmo(int32 Ammo) 
{
    ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;

    bool bHUDValid = 
        ShooterHUD && 
        ShooterHUD->CharacterOverlay && 
        ShooterHUD->CharacterOverlay->WeaponAmmoAmount;

    if(bHUDValid)
    {

        FString WeaponAmmoText = FString::Printf(TEXT("%d"), Ammo);
        ShooterHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(WeaponAmmoText));

    }
}

void AShooterPlayerController::SetHUDCarriedAmmo(int32 Ammo) 
{
    ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;

    bool bHUDValid = 
        ShooterHUD && 
        ShooterHUD->CharacterOverlay && 
        ShooterHUD->CharacterOverlay->CarriedAmmoAmount;

    if(bHUDValid)
    {

        FString CarriedAmmoText = FString::Printf(TEXT("%d"), Ammo);
        ShooterHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(CarriedAmmoText));

    }
}

void AShooterPlayerController::SetHUDMatchCountdown(float CountdownTime) 
{
    ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;

    bool bHUDValid = 
        ShooterHUD && 
        ShooterHUD->CharacterOverlay && 
        ShooterHUD->CharacterOverlay->MatchCountdownText;

    if(bHUDValid)
    {
        if(CountdownTime < 0.f)
        {
            ShooterHUD->CharacterOverlay->MatchCountdownText->SetText(FText());
            return;
        }

        int32 Minutes = FMath::FloorToInt32(CountdownTime / 60.f);
        int32 Seconds = CountdownTime - (Minutes * 60);

        FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
        if(Minutes == 0 && Seconds % 2 == 0 && Seconds <= 30)
        {
            ShooterHUD->CharacterOverlay->MatchCountdownText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
        }
        else 
        {
            ShooterHUD->CharacterOverlay->MatchCountdownText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
        }
        ShooterHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(CountdownText));

    }
}

void AShooterPlayerController::SetHUDAnnouncementCountdown(float CountdownTime) 
{
    ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;

    bool bHUDValid = 
        ShooterHUD && 
        ShooterHUD->Announcement &&
        ShooterHUD->Announcement->WarmupTime;

    if(bHUDValid)
    {

        if(CountdownTime < 0.f)
        {
            ShooterHUD->Announcement->WarmupTime->SetText(FText());
            return;
        }

        int32 Minutes = FMath::FloorToInt32(CountdownTime / 60.f);
        int32 Seconds = CountdownTime - (Minutes * 60);

        FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
        ShooterHUD->Announcement->WarmupTime->SetText(FText::FromString(CountdownText));

    }
}

void AShooterPlayerController::SetHUDTime() 
{
    float TimeLeft = 0.f;
    if(MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
    else if(MatchState == MatchState::InProgress) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
    else if(MatchState == MatchState::Cooldown) TimeLeft = WarmupTime + MatchTime + CooldownTime - GetServerTime() + LevelStartingTime;
    
    uint32 SecondsLeft = FMath::CeilToInt32(TimeLeft);

    if(HasAuthority())
    {
        ShooterGameMode = ShooterGameMode == nullptr ? Cast<AShooterGameMode>(UGameplayStatics::GetGameMode(this)) : ShooterGameMode;
        if(ShooterGameMode)
        {
            SecondsLeft = FMath::CeilToInt32(ShooterGameMode->GetCountdownTime() + LevelStartingTime);
        }
    }

    if(CountdownInt != SecondsLeft)
    {
        if(MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown)
        {
            SetHUDAnnouncementCountdown(TimeLeft);
        }
        if(MatchState == MatchState::InProgress)
        {
            SetHUDMatchCountdown(TimeLeft);
        }
    }

    CountdownInt = SecondsLeft;
}

void AShooterPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest) 
{
    float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
    ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void AShooterPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerRecievedClientRequest) 
{
    float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
    float CurrentServerTime = TimeServerRecievedClientRequest + (0.5f * RoundTripTime);
    ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

float AShooterPlayerController::GetServerTime() 
{
    if(!HasAuthority())
    {
        return GetWorld()->GetTimeSeconds() + ClientServerDelta;
    }
    else
    {
        return GetWorld()->GetTimeSeconds();
    }
    
}

void AShooterPlayerController::ReceivedPlayer() 
{
    Super:: ReceivedPlayer();

    if(IsLocalController())
    {
        ServerRequestServerTime(GetWorld()->GetTimeSeconds());
    }
}

void AShooterPlayerController::OnMatchStateSet(FName State) 
{
    MatchState = State;

    if(MatchState == MatchState::InProgress)
    {
        HandleMatchHasStarted();
    }
    else if(MatchState == MatchState::Cooldown)
    {
        HandleCooldown();
    }
}

void AShooterPlayerController::OnRep_MatchState() 
{
    if(MatchState == MatchState::InProgress)
    {
        HandleMatchHasStarted();
    }
    else if(MatchState == MatchState::Cooldown)
    {
        HandleCooldown();
    }
}

void AShooterPlayerController::HandleMatchHasStarted() 
{
    ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;
    if(ShooterHUD)
    {
        if(ShooterHUD->Announcement)
        {
            ShooterHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
        }
        ShooterHUD->AddCharacterOverlay();
    }
}

void AShooterPlayerController::HandleCooldown() 
{
    ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;
    if(ShooterHUD)
    {
        ShooterHUD->CharacterOverlay->RemoveFromParent();

        bool bHUDValid = ShooterHUD->Announcement && 
                         ShooterHUD->Announcement->AnnouncementText && 
                         ShooterHUD->Announcement->InfoText;

        if(bHUDValid)
        {
            ShooterHUD->Announcement->SetVisibility(ESlateVisibility::Visible);
            FString AnnouncementText("New Match Starts In:");
            ShooterHUD->Announcement->AnnouncementText->SetText(FText::FromString(AnnouncementText));
            
            AShooterGameState* ShooterGameState = Cast<AShooterGameState>(UGameplayStatics::GetGameState(this));
            AShooterPlayerState* ShooterPlayerState = GetPlayerState<AShooterPlayerState>();


            if(ShooterGameState && ShooterPlayerState)
            {
                TArray<AShooterPlayerState*> TopPlayers = ShooterGameState->TopScoringPlayers;
                FString InfoTextString;

                if(TopPlayers.Num() == 0)
                {
                    InfoTextString = FString("Nobody Wins -_-");
                }
                else if(TopPlayers.Num() == 1 && TopPlayers[0] == ShooterPlayerState)
                {
                    InfoTextString = FString("You Are The Winner!");
                }
                else if (TopPlayers.Num() == 1)
                {
                    InfoTextString = FString::Printf(TEXT("Winner: \n%s"), *TopPlayers[0]->GetPlayerName());
                }
                else if (TopPlayers.Num() > 1)
                {
                    InfoTextString = FString("Players Tied For The Win:\n");
                    for (auto TiedPlayer : TopPlayers)
                    {
                        InfoTextString.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName()));
                    }
                }

                ShooterHUD->Announcement->InfoText->SetText(FText::FromString(InfoTextString));
            }

            

        }
    }
    AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(GetPawn());
    if(ShooterCharacter && ShooterCharacter->GetCombatComponent())
    {
        ShooterCharacter->bDisableGameplay = true;
        ShooterCharacter->GetCombatComponent()->FireButtonPressed(false);
    }
}
