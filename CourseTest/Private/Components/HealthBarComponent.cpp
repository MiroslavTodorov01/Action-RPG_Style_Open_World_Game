// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthBarComponent.h"
#include "Widgets/HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthBarPercentage(float Percentage)
{
	if (!HealthBarWidget)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percentage);
	}
}
