// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class COURSETEST_API ATreasure : public AItem
{
	GENERATED_BODY()

protected:
	virtual void OverlapBegin(UPrimitiveComponent* ComponentOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult) override;

private:

	UPROPERTY(EditDefaultsOnly)
	int32 GoldValue;

public:
	FORCEINLINE int32 GetGoldValue() const { return GoldValue; };
};
