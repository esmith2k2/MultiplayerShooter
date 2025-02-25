// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "ShooterGame/HUD/ShooterHUD.h"
#include "ShooterGame/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "ShooterGame/Character/ShooterCharacter.h"


    void AShooterPlayerController::BeginPlay() 
{
    Super::BeginPlay();


    ShooterHUD = Cast<AShooterHUD>(GetHUD());

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
}


