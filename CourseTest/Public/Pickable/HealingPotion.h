// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "HealingPotion.generated.h"

/**
 * 
 */
UCLASS()
class COURSETEST_API AHealingPotion : public AItem
{
	GENERATED_BODY()
	
protected:
	virtual void OverlapBegin(UPrimitiveComponent* ComponentOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere)
	float HealAmount;

public:
	FORCEINLINE float GetHealAmount() const { return HealAmount; };
	FORCEINLINE void SetHealAmount(float Value) { HealAmount = Value; };
};
