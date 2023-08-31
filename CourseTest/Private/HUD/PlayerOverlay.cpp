#include "HUD/PlayerOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerOverlay::SetHealthBarPercentage(float Percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetStaminaBarPercentage(float Percent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetGold(int32 GoldAmount)
{
	if (GoldText)
	{
		GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), GoldAmount)));
	}
}

void UPlayerOverlay::SetSouls(int32 SoulsAmount)
{
	if (SoulsText)
	{
		SoulsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), SoulsAmount)));
	}
}

void UPlayerOverlay::SetHealingPotions(int32 HealingPotionsAmount)
{
	if (HealingPotionsText)
	{
		HealingPotionsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), HealingPotionsAmount)));
	}
}
