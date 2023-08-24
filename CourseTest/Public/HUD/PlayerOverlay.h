// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class COURSETEST_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetHealthBarPercentage(float Percent);
	void SetStaminaBarPercentage(float Percent);
	void SetGold(int32 GoldAmount);
	void SetSouls(int32 SoulsAmount);

private:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GoldText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SoulsText;
};
