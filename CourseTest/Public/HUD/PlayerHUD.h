// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UPlayerOverlay;

UCLASS()
class COURSETEST_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE UPlayerOverlay* GetPlayerOverlay() { return Overlay; };

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerOverlay> ClassPlayerOverlay;

	UPlayerOverlay* Overlay;
};
