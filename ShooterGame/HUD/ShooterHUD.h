// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()

public: 
	UPROPERTY()
	class UTexture2D* CrosshairsCenter;
	UPROPERTY()
	class UTexture2D* CrosshairsLeft;
	UPROPERTY()
	class UTexture2D* CrosshairsRight;
	UPROPERTY()
	class UTexture2D* CrosshairsTop;
	UPROPERTY()
	class UTexture2D* CrosshairsBottom;
	float CrosshairSpread;
	FLinearColor CrosshairsColor;
};


/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AShooterHUD : public AHUD
{
	GENERATED_BODY()

public:

virtual void DrawHUD() override;

UPROPERTY(EditAnywhere, Category = "Player Stats")
TSubclassOf<class UUserWidget> CharacterOverlayClass;

UPROPERTY()
class UCharacterOverlay* CharacterOverlay;


protected:

	virtual void BeginPlay() override;
	void AddCharacterOverlay();


private:
	FHUDPackage HUDPackage;

	void DrawCrosshair(UTexture2D* Texture, FVector2D Viewport, FVector2D Spread, FLinearColor CrosshairColor);

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;

public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
	
};
