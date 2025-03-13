// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "ShooterGame/HUD/ShooterHUD.h"
#include "ShooterGame/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "ShooterGame/Character/ShooterCharacter.h"
#include "Net/UnrealNetwork.h"
#include "ShooterGame/GameMode/ShooterGameMode.h"


void AShooterPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const 
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AShooterPlayerController, MatchState);
}

void AShooterPlayerController::BeginPlay() 
{
    Super::BeginPlay();


    ShooterHUD = Cast<AShooterHUD>(GetHUD());

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
        int32 Minutes = FMath::FloorToInt32(CountdownTime / 60.f);
        int32 Seconds = CountdownTime - (Minutes * 60);

        FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
        ShooterHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(CountdownText));

    }
}

void AShooterPlayerController::SetHUDTime() 
{
    uint32 SecondsLeft = FMath::CeilToInt32(MatchTime - GetServerTime());

    if(CountdownInt != SecondsLeft)
    {
        SetHUDMatchCountdown(FMath::Clamp(MatchTime - GetServerTime(), 0, MatchTime));
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
        ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;

        if(ShooterHUD)
        {
            ShooterHUD->AddCharacterOverlay();
        }
    }
}

void AShooterPlayerController::OnRep_MatchState() 
{
    if(MatchState == MatchState::InProgress)
    {
        ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;

        if(ShooterHUD)
        {
            ShooterHUD->AddCharacterOverlay();
        }
    }
}

