// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()
	
public: 
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DisplayText;

	void SetDisplayText(FString TextToDisplay);

	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(APawn* InPawn);

	UFUNCTION(BlueprintCallable)
	void ShowPlayerName(APawn* InPawn);

	UPROPERTY()
	class APlayerState* PlayerState;

private:
	virtual void NativeDestruct() override;
};
