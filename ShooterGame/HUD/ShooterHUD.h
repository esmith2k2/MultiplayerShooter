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
	class UTexture2D* CrosshairsCenter;
	class UTexture2D* CrosshairsLeft;
	class UTexture2D* CrosshairsRight;
	class UTexture2D* CrosshairsTop;
	class UTexture2D* CrosshairsBottom;


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


protected:


private:
	FHUDPackage HUDPackage;

public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
	
};
