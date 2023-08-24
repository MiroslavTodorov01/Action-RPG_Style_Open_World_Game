// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class COURSETEST_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:

	class UHealthBar* HealthBarWidget;

	void SetHealthBarPercentage(float Percent);
};
